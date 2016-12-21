package org.lemurproject.sifaka.textminer;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Stack;
import java.util.StringJoiner;

import org.apache.lucene.queryparser.classic.ParseException;
import org.lemurproject.sifaka.buildindex.domain.IndexConstants;
import org.lemurproject.sifaka.buildindex.domain.IndexDetails;
import org.lemurproject.sifaka.buildindex.domain.IndexFieldProperty;
import org.lemurproject.sifaka.buildindex.domain.IndexProperties;
import org.lemurproject.sifaka.buildindex.domain.IndexProperty;
import org.lemurproject.sifaka.buildindex.util.BuildIndexUtil;
import org.lemurproject.sifaka.textanalyzer.domain.CoOccurrence;
import org.lemurproject.sifaka.textanalyzer.domain.DisplayDocument;
import org.lemurproject.sifaka.textanalyzer.domain.Feature;
import org.lemurproject.sifaka.textanalyzer.domain.TermFrequency;
import org.lemurproject.sifaka.textanalyzer.service.CoOccurrenceLuceneServiceImpl;
import org.lemurproject.sifaka.textanalyzer.service.CoOccurrenceService;
import org.lemurproject.sifaka.textanalyzer.service.FeatureVectorLuceneServiceImpl;
import org.lemurproject.sifaka.textanalyzer.service.FeatureVectorService;
import org.lemurproject.sifaka.textanalyzer.service.FrequencyLuceneServiceImpl;
import org.lemurproject.sifaka.textanalyzer.service.FrequencyService;
import org.lemurproject.sifaka.textanalyzer.service.SearchLuceneServiceImpl;
import org.lemurproject.sifaka.textanalyzer.service.SearchService;
import org.lemurproject.sifaka.textanalyzer.util.CSVUtilImpl;
import org.lemurproject.sifaka.textminer.domain.IndexTreeItem;

import javafx.application.Application;
import javafx.beans.binding.Bindings;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.concurrent.Task;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ComboBox;
import javafx.scene.control.ContextMenu;
import javafx.scene.control.Hyperlink;
import javafx.scene.control.Label;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.Pagination;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.SeparatorMenuItem;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableRow;
import javafx.scene.control.TableView;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleGroup;
import javafx.scene.control.Tooltip;
import javafx.scene.control.TreeCell;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.scene.web.WebEngine;
import javafx.scene.web.WebView;
import javafx.stage.DirectoryChooser;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;
import javafx.stage.Stage;
import javafx.stage.StageStyle;
import javafx.util.Callback;

public class TextMiner extends Application {

  private static final String APP_NAME = "Sifaka Text Miner";
  private static String searchEngineType;

  private static final Font LABEL_FONT = Font.font("Arial", FontWeight.BOLD, 12);
  private static final Font WARNING_FONT = Font.font("Arial", FontWeight.BOLD, 12);
  private static final Font RESULT_NUM_FONT = Font.font("Arial", 12);

  private CSVUtilImpl csvUtil;
  private SearchService searchService;
  private FrequencyService frequencyService;
  private CoOccurrenceService coOccurrenceService;
  private FeatureVectorService featureVectorService;

  private File previousIndexDirectory = null;
  private BorderPane searchScene = null;
  private Tab searchTab = null;
  private TextField searchField = null;

  private List<Integer> hits = null;
  private List<TermFrequency> termsAndFreqs = null;
  private List<CoOccurrence> coOccurrences = null;
  private Map<String, List<Feature>> featureVectors = null;

  public static void main(String[] args) {
    searchEngineType = "Lucene";
    launch(args);
  }

  @Override
  public void start(Stage primaryStage) throws Exception {
    initializeServices();
    primaryStage.setTitle(APP_NAME);

    BorderPane bp = new BorderPane();

    // Navigation -------------------------------------------------
    TreeItem<IndexTreeItem> rootItem = createLeftNav(bp, primaryStage);

    // Menu -------------------------------------------------------
    createMenu(rootItem, bp, primaryStage);

    primaryStage.setScene(new Scene(bp, 1000, 800));
    primaryStage.show();
  }

  private void initializeServices() {
    csvUtil = new CSVUtilImpl();
    if (searchEngineType.equals("Lucene")) {
      searchService = new SearchLuceneServiceImpl();
      frequencyService = new FrequencyLuceneServiceImpl();
      coOccurrenceService = new CoOccurrenceLuceneServiceImpl();
      featureVectorService = new FeatureVectorLuceneServiceImpl();
    }
  }

  private void createMenu(TreeItem<IndexTreeItem> rootItem, BorderPane bp, Stage primaryStage) {
    VBox topContainer = new VBox();
    MenuBar menuBar = new MenuBar();
    Menu fileMenu = new Menu("File");

    Stack<IndexTreeItem> stack = getRecentlyOpenedIndexes(fileMenu);

    MenuItem aboutItem = new MenuItem("About...");
    aboutItem.setOnAction(new EventHandler<ActionEvent>() {
      @Override
      public void handle(ActionEvent event) {
        Stage stage = new Stage();
        stage.setTitle("About");

        VBox root = new VBox();

        final WebView browser = new WebView();
        final WebEngine webEngine = browser.getEngine();

        ScrollPane scrollPane = new ScrollPane();
        scrollPane.setContent(browser);
        String aboutContent;
        try {
          aboutContent = readFile("abouttextminig.html");
          webEngine.loadContent(aboutContent);
        } catch (IOException e) {
          System.out.println("Could not load ABOUT.html");
        }

        root.getChildren().addAll(scrollPane);

        Scene scene = new Scene(root);
        stage.setScene(scene);

        stage.show();

      }
    });
    fileMenu.getItems().add(aboutItem);

    MenuItem importIndex = new MenuItem("Open Index");
    importIndex.setOnAction(new EventHandler<ActionEvent>() {

      @Override
      public void handle(ActionEvent event) {
        DirectoryChooser fileChooser = new DirectoryChooser();
        fileChooser.setTitle("Open Index");
        if (previousIndexDirectory != null) {
          fileChooser.setInitialDirectory(previousIndexDirectory);
        }
        File indexDirectory = fileChooser.showDialog(primaryStage);
        if (indexDirectory != null) {
          previousIndexDirectory = indexDirectory.getParentFile();
        }

        try {
          BuildIndexUtil indexUtil = new BuildIndexUtil();
          IndexProperties indexProperties = indexUtil.getIndexProperties(indexDirectory.getPath(),
              IndexConstants.LUCENE_INDEX_PROPERTIES_FILE);

          if (indexProperties == null) {
            throw new IOException();
          }

          // Update Index location
          indexProperties.getIndexDetails().setBasePath(indexDirectory.getAbsolutePath());
          indexProperties.getIndexDetails().setSearchDataPath(
              Paths.get(indexDirectory.getAbsolutePath(), IndexConstants.LUCENE_INDEX_DIRECTORY)
                  .toString());
          indexProperties.getBasicIndexProperties().add(0,
              new IndexProperty("Path", indexDirectory.getAbsolutePath()));

          IndexTreeItem item = new IndexTreeItem();
          item.setIndexProperties(indexProperties);
          item.setName(indexDirectory.getName());
          item.setIndex(true);

          if (stack.contains(item)) {
            stack.remove(item);
          }
          stack.push(item);
          FileOutputStream fout;

          fout = new FileOutputStream("RecentIndexes.txt");
          ObjectOutputStream oos = new ObjectOutputStream(fout);
          oos.writeObject(stack);
          oos.close();

          TreeItem<IndexTreeItem> indexItem = new TreeItem<IndexTreeItem>(item);
          rootItem.getChildren().add(indexItem);
        } catch (IOException e) {
          createWarningsPopup(
              "Could not open index at: " + indexDirectory.getPath()
                  + "\n\nMake sure the directory selected contains\nan index directory and manifest file",
              primaryStage);
        }


      }
    });
    fileMenu.getItems().add(importIndex);

    fileMenu.getItems().add(new SeparatorMenuItem());

    Iterator<IndexTreeItem> iterator = stack.iterator();
    int i = 1;
    while (iterator.hasNext() && i <= 10) {
      IndexTreeItem treeItem = iterator.next();
      MenuItem recentIndex = new MenuItem(String.valueOf(i) + " " + treeItem.getName());
      fileMenu.getItems().add(recentIndex);
      i++;

      recentIndex.setOnAction(new EventHandler<ActionEvent>() {
        @Override
        public void handle(ActionEvent event) {
          TreeItem<IndexTreeItem> indexItem = new TreeItem<IndexTreeItem>(treeItem);
          rootItem.getChildren().add(indexItem);
        }
      });
    }

    menuBar.getMenus().addAll(fileMenu);
    topContainer.getChildren().add(menuBar);
    bp.setTop(topContainer);
  }

  private String readFile(String pathString) throws IOException {
    BufferedReader reader =
        new BufferedReader(new InputStreamReader(getClass().getResourceAsStream("/" + pathString)));
    StringJoiner fileString = new StringJoiner("");
    String line;
    while ((line = reader.readLine()) != null) {
      fileString.add(line);
    }
    return fileString.toString();
  }

  private Stack<IndexTreeItem> getRecentlyOpenedIndexes(Menu fileMenu) {
    Stack<IndexTreeItem> stack = new Stack<>();

    File recentIndexesFile = new File("RecentIndexes.txt");
    if (recentIndexesFile.exists()) {
      try {
        // use buffering
        InputStream file = new FileInputStream("RecentIndexes.txt");
        InputStream buffer = new BufferedInputStream(file);
        ObjectInput input = new ObjectInputStream(buffer);
        try {
          // deserialize the List
          stack = (Stack<IndexTreeItem>) input.readObject();
        } finally {
          input.close();
        }
      } catch (ClassNotFoundException ex) {
        System.out.println("Recent Index Class not found Error");
      } catch (IOException ex) {
        System.out.println("RecentIndex IO Error");
      }
    }

    return stack;
  }

  private TreeItem<IndexTreeItem> createLeftNav(BorderPane bp, Stage primaryStage) {
    IndexTreeItem rootTreeItem = new IndexTreeItem();
    rootTreeItem.setName("Indexes");
    rootTreeItem.setIndex(false);
    TreeItem<IndexTreeItem> rootItem = new TreeItem<IndexTreeItem>(rootTreeItem);
    rootItem.setExpanded(true);
    TreeView<IndexTreeItem> tree = new TreeView<IndexTreeItem>(rootItem);
    tree.setCellFactory(new Callback<TreeView<IndexTreeItem>, TreeCell<IndexTreeItem>>() {

      @Override
      public TreeCell<IndexTreeItem> call(TreeView<IndexTreeItem> param) {
        return new IndexTreeCell();
      }
    });

    tree.setOnMouseClicked(new EventHandler<MouseEvent>() {

      @Override
      public void handle(MouseEvent event) {
        // if (event.getClickCount() == 2) {
        TreeItem<IndexTreeItem> item = tree.getSelectionModel().getSelectedItem();
        if (!item.getValue().getName().equals(rootTreeItem.getName())) {
          // Tabs -------------------------------------------------
          TabPane tabPane = new TabPane();

          if (item.getValue().getSet() != null) {
            createSetPropertiesTab(tabPane, item);
          }

          createPropertiesTab(tabPane, item);

          createSearchTab(tabPane, item);

          creatAnalysisTab(tabPane, item, primaryStage);

          creatCoOccurrenceTab(tabPane, item, primaryStage);

          if (item.getValue().getIndexProperties().getIndexDetails().getLabelFields() != null) {
            createFeatureVectorTab(tabPane, item, primaryStage);
          }

          bp.setCenter(tabPane);
          // End Tabs ------------------------------------------------

        } else {
          bp.setCenter(new Pane());
        }

      }

    });

    bp.setLeft(tree);
    return rootItem;
  }

  private void createSetPropertiesTab(TabPane tabPane, TreeItem<IndexTreeItem> item) {
    Tab propertiesTab = new Tab();
    propertiesTab.setText("Set Properties");
    propertiesTab.setClosable(false);

    TableView<IndexProperty> table = new TableView<>();
    TableColumn<IndexProperty, String> nameColumn = new TableColumn<IndexProperty, String>("Name");
    nameColumn.setCellValueFactory(new PropertyValueFactory<IndexProperty, String>("name"));
    nameColumn.setMinWidth(200);
    table.getColumns().add(nameColumn);
    TableColumn<IndexProperty, String> valueColumn =
        new TableColumn<IndexProperty, String>("Value");
    valueColumn.setCellValueFactory(new PropertyValueFactory<IndexProperty, String>("value"));
    table.getColumns().add(valueColumn);

    List<IndexProperty> properties = new ArrayList<IndexProperty>();

    IndexProperty setName = new IndexProperty("Set Name", item.getValue().getName());
    properties.add(setName);

    IndexProperty setQuery =
        new IndexProperty("Query searched to create set", item.getValue().getSetQueryText());
    properties.add(setQuery);

    IndexProperty numDocs =
        new IndexProperty("Number of Documents", String.valueOf(item.getValue().getSet().size()));
    properties.add(numDocs);

    table.getItems().addAll(properties);
    propertiesTab.setContent(table);

    tabPane.getTabs().add(propertiesTab);
  }

  private void createPropertiesTab(TabPane tabPane, TreeItem<IndexTreeItem> item) {
    Tab propertiesTab = new Tab();
    propertiesTab.setText("Index Properties");
    propertiesTab.setClosable(false);

    VBox propertiesLayout = new VBox();

    TableView<IndexProperty> table = new TableView<>();
    TableColumn<IndexProperty, String> nameColumn = new TableColumn<IndexProperty, String>("Name");
    nameColumn.setCellValueFactory(new PropertyValueFactory<IndexProperty, String>("name"));
    nameColumn.setMinWidth(200);
    table.getColumns().add(nameColumn);
    TableColumn<IndexProperty, String> valueColumn =
        new TableColumn<IndexProperty, String>("Value");
    valueColumn.setCellValueFactory(new PropertyValueFactory<IndexProperty, String>("value"));
    table.getColumns().add(valueColumn);
    table.setFixedCellSize(25);
    table.minHeightProperty()
        .bind(Bindings.size(table.getItems()).multiply(table.getFixedCellSize()).add(30));

    List<IndexProperty> properties = item.getValue().getIndexProperties().getBasicIndexProperties();

    table.getItems().addAll(properties);

    // table.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
    propertiesLayout.getChildren().add(table);

    TableView<IndexFieldProperty> fieldPropertiesTable = new TableView<>();
    TableColumn<IndexFieldProperty, String> fieldColumn =
        new TableColumn<IndexFieldProperty, String>("Field");
    fieldColumn.setCellValueFactory(new PropertyValueFactory<IndexFieldProperty, String>("field"));
    fieldPropertiesTable.getColumns().add(fieldColumn);
    TableColumn<IndexFieldProperty, String> annotationColumn =
        new TableColumn<IndexFieldProperty, String>("Annotation");
    annotationColumn
        .setCellValueFactory(new PropertyValueFactory<IndexFieldProperty, String>("annotation"));
    fieldPropertiesTable.getColumns().add(annotationColumn);
    TableColumn<IndexFieldProperty, String> countColumn =
        new TableColumn<IndexFieldProperty, String>("Count");
    countColumn.setCellValueFactory(new PropertyValueFactory<IndexFieldProperty, String>("count"));
    fieldPropertiesTable.getColumns().add(countColumn);
    fieldPropertiesTable.setFixedCellSize(25);

    List<IndexFieldProperty> fieldProperties =
        item.getValue().getIndexProperties().getIndexFieldProperties();
    fieldPropertiesTable.getItems().addAll(fieldProperties);
    fieldPropertiesTable.prefHeightProperty().bind(Bindings.size(fieldPropertiesTable.getItems())
        .multiply(fieldPropertiesTable.getFixedCellSize()).add(30));
    propertiesLayout.getChildren().add(fieldPropertiesTable);

    propertiesTab.setContent(propertiesLayout);

    tabPane.getTabs().add(propertiesTab);
  }

  private void creatCoOccurrenceTab(TabPane tabPane, TreeItem<IndexTreeItem> item,
      Stage primaryStage) {
    Tab coTab = new Tab();
    coTab.setText("Co-occurrence");
    coTab.setClosable(false);

    HBox scene = new HBox();
    VBox inputScene = new VBox();
    inputScene.setPadding(new Insets(10, 10, 10, 10));
    inputScene.setSpacing(20);
    GridPane inputGrid = new GridPane();
    inputGrid.setVgap(5);

    Label label1 = new Label("Query:  ");
    label1.setFont(LABEL_FONT);;
    inputGrid.add(label1, 0, 0);
    TextField textField1 = new TextField();
    inputGrid.add(textField1, 1, 0);

    Label label2 = new Label("Result Type:  ");
    label2.setFont(LABEL_FONT);
    inputGrid.add(label2, 0, 1);
    ComboBox<String> resultCombo = new ComboBox<>();
    List<String> annotationTypes =
        item.getValue().getIndexProperties().getIndexDetails().getAnnotationTypes();
    if (annotationTypes != null && annotationTypes.size() > 0) {
      resultCombo.getItems().addAll(annotationTypes);
      resultCombo.getSelectionModel().select(annotationTypes.get(0));
    }
    inputGrid.add(resultCombo, 1, 1);

    Label label3 = new Label("Search Depth:  ");
    label3.setFont(LABEL_FONT);
    inputGrid.add(label3, 0, 2);
    TextField textField3 = new TextField();
    inputGrid.add(textField3, 1, 2);
    Tooltip depthTooltip = new Tooltip();
    depthTooltip.setText("The number of documents \nreturned from the query.");
    Tooltip.install(textField3, depthTooltip);
    Tooltip.install(label3, depthTooltip);
    label3.setTooltip(depthTooltip);

    Label label4 = new Label("Minimum Frequency:  ");
    label4.setFont(LABEL_FONT);
    inputGrid.add(label4, 0, 3);
    TextField textField4 = new TextField();
    Tooltip minFreqTooltip = new Tooltip();
    minFreqTooltip
        .setText("The minimum number of times the result type \nmust occur in the search results");
    Tooltip.install(label4, minFreqTooltip);
    Tooltip.install(textField4, minFreqTooltip);
    inputGrid.add(textField4, 1, 3);

    Label label5 = new Label("Number of Results:  ");
    label5.setFont(LABEL_FONT);
    inputGrid.add(label5, 0, 4);
    TextField textField5 = new TextField();
    Tooltip numResultsTooltip = new Tooltip();
    numResultsTooltip.setText("The top number of entities that co-occur with the query");
    Tooltip.install(label5, numResultsTooltip);
    Tooltip.install(textField5, numResultsTooltip);
    inputGrid.add(textField5, 1, 4);

    Label sortLabel = new Label("Sort By:  ");
    sortLabel.setFont(LABEL_FONT);
    inputGrid.add(sortLabel, 0, 5);

    final ToggleGroup sortGroup = new ToggleGroup();

    RadioButton df = new RadioButton("Term Frequency");
    df.setToggleGroup(sortGroup);
    df.setSelected(true);
    inputGrid.add(df, 0, 6);

    RadioButton pmi = new RadioButton("PMI");
    pmi.setToggleGroup(sortGroup);
    inputGrid.add(pmi, 0, 7);

    RadioButton phi = new RadioButton("Phi-Square");
    phi.setToggleGroup(sortGroup);
    inputGrid.add(phi, 0, 8);

    Button submit = new Button("Submit");
    inputGrid.add(submit, 0, 10);

    Button clear = new Button("Clear");
    inputGrid.add(clear, 1, 10);

    inputScene.getChildren().add(inputGrid);
    scene.getChildren().add(inputScene);

    BorderPane resultsLayout = new BorderPane();
    resultsLayout.setVisible(false);

    TableView<CoOccurrence> table = new TableView<>();
    table.setMinWidth(410);
    TableColumn<CoOccurrence, String> termColumn = new TableColumn<CoOccurrence, String>("Term");
    termColumn.setCellValueFactory(new PropertyValueFactory<CoOccurrence, String>("term"));
    termColumn.setMinWidth(200);
    table.getColumns().add(termColumn);
    TableColumn<CoOccurrence, String> dfColumn = new TableColumn<CoOccurrence, String>("Term Freq");
    dfColumn.setCellValueFactory(new PropertyValueFactory<CoOccurrence, String>("termFrequency"));
    table.getColumns().add(dfColumn);
    TableColumn<CoOccurrence, String> pmiColumn = new TableColumn<CoOccurrence, String>("PMI");
    pmiColumn.setCellValueFactory(new PropertyValueFactory<CoOccurrence, String>("pmi"));
    table.getColumns().add(pmiColumn);
    TableColumn<CoOccurrence, String> phisquareColumn =
        new TableColumn<CoOccurrence, String>("Phi-Square");
    phisquareColumn
        .setCellValueFactory(new PropertyValueFactory<CoOccurrence, String>("phisquare"));
    table.getColumns().add(phisquareColumn);
    resultsLayout.setCenter(table);

    table.setRowFactory(new Callback<TableView<CoOccurrence>, TableRow<CoOccurrence>>() {

      @Override
      public TableRow<CoOccurrence> call(TableView<CoOccurrence> table) {
        final TableRow<CoOccurrence> row = new TableRow<>();
        final ContextMenu searchTermMenu = new ContextMenu();
        final MenuItem searchTermMenuItem = new MenuItem("Search");
        searchTermMenuItem.setOnAction(new EventHandler<ActionEvent>() {

          @Override
          public void handle(ActionEvent event) {
            switchToSearchTab(tabPane, row.getItem().getTerm(), item);
          }

        });
        searchTermMenu.getItems().add(searchTermMenuItem);
        row.contextMenuProperty().bind(
            Bindings.when(row.emptyProperty()).then((ContextMenu) null).otherwise(searchTermMenu));
        return row;
      }

    });

    Button saveResults = new Button("Save Results");
    resultsLayout.setBottom(saveResults);
    scene.getChildren().add(resultsLayout);

    submit.setOnAction(new EventHandler<ActionEvent>() {

      @Override
      public void handle(ActionEvent event) {
        // Remove existing table
        // scene.getChildren().remove(table);
        table.getItems().clear();
        // Remove existing warnings
        if (inputScene.getChildren().size() > 1) {
          inputScene.getChildren().remove(1);
        }
        ProgressForm pForm = new ProgressForm(primaryStage);

        Task<Void> task = new Task<Void>() {
          @Override
          public Void call() throws InterruptedException {
            if (textField1.getText() != null && textField1.getText().length() > 0
                && validateInputIsInteger(textField3.getText())
                && validateInputIsInteger(textField4.getText())
                && validateInputIsInteger(textField5.getText())) {
              RadioButton sortByResult = (RadioButton) sortGroup.getSelectedToggle();
              try {
                coOccurrences = coOccurrenceService.coOccurrence(
                    item.getValue().getIndexProperties().getIndexDetails(),
                    item.getValue().getSet(), textField1.getText(), resultCombo.getValue(),
                    Integer.valueOf(textField3.getText()).intValue(),
                    Integer.valueOf(textField4.getText()).intValue(),
                    Integer.valueOf(textField5.getText()).intValue(), sortByResult.getText());
              } catch (IOException | ParseException e) {
                System.out.println("Error calling query service");
              }
            }
            return null;
          }
        };
        // binds progress of progress bars to progress of task:
        pForm.activateProgressBar(task);

        // in real life this method would get the result of the task
        // and update the UI based on its value:
        task.setOnSucceeded(event2 -> {
          pForm.getDialogStage().close();
          submit.setDisable(false);
          if (validateInputIsInteger(textField3.getText())
              && validateInputIsInteger(textField4.getText())
              && validateInputIsInteger(textField5.getText())) {
            table.getItems().addAll(coOccurrences);
            table.refresh();
            resultsLayout.setVisible(true);
          } else {
            StringJoiner warningJoiner = new StringJoiner("\n");
            if (textField1.getText() == null || textField1.getText().trim().length() == 0) {
              warningJoiner.add("You must enter a query\n");
            }
            if (!validateInputIsInteger(textField3.getText())) {
              warningJoiner.add(
                  "Search Depth must be and integer between \n1 and the total number of documents\n");
            }
            if (!validateInputIsInteger(textField4.getText())) {
              warningJoiner.add("Minimum Frequency must be a positive integer\n");
            }
            if (!validateInputIsInteger(textField5.getText())) {
              warningJoiner.add("Num Results must be a positive integer\n");
            }
            // createWarningsPopup(warningJoiner.toString(), primaryStage);
            Text textWarning = new Text(warningJoiner.toString());
            textWarning.setFont(WARNING_FONT);
            textWarning.setFill(Color.DARKRED);
            inputScene.getChildren().add(textWarning);
          }
        });

        submit.setDisable(true);
        pForm.getDialogStage().show();

        Thread thread = new Thread(task);
        thread.start();
      }

    });

    clear.setOnAction(new EventHandler<ActionEvent>() {
      @Override
      public void handle(ActionEvent event) {
        textField1.clear();
        textField3.clear();
        textField4.clear();
        textField5.clear();
      }
    });

    saveResults.setOnAction(new EventHandler<ActionEvent>() {

      @Override
      public void handle(ActionEvent event) {
        FileChooser fileChooser = new FileChooser();
        fileChooser.getExtensionFilters().add(new ExtensionFilter("CSV", "*.csv"));
        fileChooser.setTitle("Save Results");
        File resultsFile = fileChooser.showSaveDialog(primaryStage);
        csvUtil.writeContentToCSVFile(resultsFile, coOccurrences);
      }

    });

    coTab.setContent(scene);

    tabPane.getTabs().add(coTab);
  }

  private void creatAnalysisTab(TabPane tabPane, TreeItem<IndexTreeItem> item, Stage primaryStage) {
    Tab coTab = new Tab();
    coTab.setText("Frequency");
    coTab.setClosable(false);

    HBox scene = new HBox();
    VBox inputScene = new VBox();
    inputScene.setPadding(new Insets(10, 10, 10, 10));
    inputScene.setSpacing(20);
    GridPane inputGrid = new GridPane();
    inputGrid.setVgap(5);

    final ToggleGroup group = new ToggleGroup();

    Label entityLabel = new Label("Find Top entities of type:  ");
    entityLabel.setFont(LABEL_FONT);
    inputGrid.add(entityLabel, 0, 0);

    int rowNum = 1;
    boolean selected = false;
    List<String> annotationTypes =
        item.getValue().getIndexProperties().getIndexDetails().getAnnotationTypes();
    if (annotationTypes != null) {
      for (String annotationType : annotationTypes) {
        RadioButton person = new RadioButton(annotationType);
        if (!selected) {
          person.setSelected(true);
          selected = true;
        }
        person.setToggleGroup(group);
        inputGrid.add(person, 0, rowNum);
        rowNum++;
      }
    }
    rowNum++;

    Label label1 = new Label("Number of Results:  ");
    label1.setFont(LABEL_FONT);
    inputGrid.add(label1, 0, rowNum);
    TextField numValuesField = new TextField();
    inputGrid.add(numValuesField, 1, rowNum);
    rowNum++;
    rowNum++;

    Label sortLabel = new Label("Sort By:  ");
    sortLabel.setFont(LABEL_FONT);
    inputGrid.add(sortLabel, 0, rowNum);
    rowNum++;

    final ToggleGroup sortGroup = new ToggleGroup();

    RadioButton df = new RadioButton("Document Frequency");
    df.setToggleGroup(sortGroup);
    df.setSelected(true);
    inputGrid.add(df, 0, rowNum);
    rowNum++;

    RadioButton ctf = new RadioButton("Collection Term Frequency");
    ctf.setToggleGroup(sortGroup);
    inputGrid.add(ctf, 0, rowNum);
    rowNum++;
    rowNum++;

    Button submit = new Button("Submit");
    inputGrid.add(submit, 0, rowNum);
    rowNum++;

    inputScene.getChildren().add(inputGrid);
    scene.getChildren().add(inputScene);

    BorderPane resultsLayout = new BorderPane();
    resultsLayout.setVisible(false);

    TableView<TermFrequency> table = new TableView<>();
    table.setMinWidth(410);
    TableColumn<TermFrequency, String> termColumn = new TableColumn<TermFrequency, String>("Term");
    termColumn.setCellValueFactory(new PropertyValueFactory<TermFrequency, String>("term"));
    termColumn.setMinWidth(200);
    table.getColumns().add(termColumn);
    TableColumn<TermFrequency, String> dfColumn =
        new TableColumn<TermFrequency, String>("Document Freq");
    dfColumn
        .setCellValueFactory(new PropertyValueFactory<TermFrequency, String>("documentFrequency"));
    table.getColumns().add(dfColumn);
    TableColumn<TermFrequency, String> ctfColumn =
        new TableColumn<TermFrequency, String>("Collection Freq");
    ctfColumn.setCellValueFactory(
        new PropertyValueFactory<TermFrequency, String>("collectionTermFrequency"));
    table.getColumns().add(ctfColumn);
    resultsLayout.setCenter(table);

    table.setRowFactory(new Callback<TableView<TermFrequency>, TableRow<TermFrequency>>() {

      @Override
      public TableRow<TermFrequency> call(TableView<TermFrequency> table) {
        final TableRow<TermFrequency> row = new TableRow<>();
        final ContextMenu searchTermMenu = new ContextMenu();
        final MenuItem searchTermMenuItem = new MenuItem("Search");
        searchTermMenuItem.setOnAction(new EventHandler<ActionEvent>() {

          @Override
          public void handle(ActionEvent event) {
            switchToSearchTab(tabPane, row.getItem().getTerm(), item);
          }

        });
        searchTermMenu.getItems().add(searchTermMenuItem);
        row.contextMenuProperty().bind(
            Bindings.when(row.emptyProperty()).then((ContextMenu) null).otherwise(searchTermMenu));
        return row;
      }

    });

    Button saveResults = new Button("Save Results");
    resultsLayout.setBottom(saveResults);

    scene.getChildren().add(resultsLayout);

    submit.setOnAction(new EventHandler<ActionEvent>() {

      @Override
      public void handle(ActionEvent event) {
        // Remove existing table
        table.getItems().clear();
        // Remove existing warnings
        if (inputScene.getChildren().size() > 1) {
          inputScene.getChildren().remove(1);
        }
        ProgressForm pForm = new ProgressForm(primaryStage);

        Task<Void> task = new Task<Void>() {
          @Override
          public Void call() throws InterruptedException {
            RadioButton check = (RadioButton) group.getSelectedToggle();
            if (validateInputIsInteger(numValuesField.getText())) {
              Integer inputNumResults = Integer.valueOf(numValuesField.getText());
              int numResults = 10;
              if (inputNumResults != null) {
                numResults = inputNumResults.intValue();
              }
              RadioButton sortByResult = (RadioButton) sortGroup.getSelectedToggle();
              try {
                termsAndFreqs = frequencyService.getFrequencies(
                    item.getValue().getIndexProperties().getIndexDetails(),
                    item.getValue().getSet(), check.getText(), numResults, sortByResult.getText());
              } catch (IOException e) {
                System.out.println("Error calling query service");
                e.printStackTrace();
              }
            }
            return null;
          }
        };
        // binds progress of progress bars to progress of task:
        pForm.activateProgressBar(task);

        // in real life this method would get the result of the task
        // and update the UI based on its value:
        task.setOnSucceeded(event2 -> {
          pForm.getDialogStage().close();
          submit.setDisable(false);
          if (validateInputIsInteger(numValuesField.getText())) {
            table.getItems().addAll(termsAndFreqs);
            table.refresh();
            resultsLayout.setVisible(true);
          } else {
            // createWarningsPopup("Number of values must be a positive integer", primaryStage);
            Text textWarning = new Text("Number of values must be a positive integer");
            textWarning.setFont(WARNING_FONT);
            textWarning.setFill(Color.DARKRED);
            inputScene.getChildren().add(textWarning);
          }
        });

        submit.setDisable(true);
        pForm.getDialogStage().show();

        Thread thread = new Thread(task);
        thread.start();
      }

    });

    saveResults.setOnAction(new EventHandler<ActionEvent>() {

      @Override
      public void handle(ActionEvent event) {
        FileChooser fileChooser = new FileChooser();
        fileChooser.getExtensionFilters().add(new ExtensionFilter("CSV", "*.csv"));
        fileChooser.setTitle("Save Results");
        File resultsFile = fileChooser.showSaveDialog(primaryStage);
        csvUtil.writeContentToCSVFile(resultsFile, termsAndFreqs);
      }

    });

    coTab.setContent(scene);

    tabPane.getTabs().add(coTab);
  }

  private void createSearchTab(TabPane tabPane, TreeItem<IndexTreeItem> item) {
    searchTab = new Tab();
    searchTab.setText("Search");
    searchTab.setClosable(false);

    searchScene = new BorderPane();
    HBox input = new HBox();

    searchField = new TextField();
    searchField.setPrefWidth(300.0);
    input.getChildren().add(searchField);

    Button submit = new Button("Search");
    input.getChildren().add(submit);

    Button saveSearch = new Button("Save Search Results");
    input.getChildren().add(saveSearch);

    searchScene.setTop(input);

    submit.setOnAction(new EventHandler<ActionEvent>() {
      @Override
      public void handle(ActionEvent event) {
        try {
          hits = searchService.search(item.getValue().getIndexProperties().getIndexDetails(),
              item.getValue().getSet(), searchField.getText());
          searchScene.setCenter(createSearchResultsContent(
              item.getValue().getIndexProperties().getIndexDetails(), hits, searchField.getText()));
        } catch (IOException | ParseException e) {
          System.out.println("Error calling query service");
          e.printStackTrace();
        }
      }
    });

    saveSearch.setOnAction(new EventHandler<ActionEvent>() {
      @Override
      public void handle(ActionEvent event) {
        Stage stage = new Stage();
        stage.centerOnScreen();
        stage.setTitle("Create Saved Set");
        VBox vbox = new VBox();
        vbox.setAlignment(Pos.CENTER);
        HBox hbox = new HBox();
        hbox.setAlignment(Pos.CENTER);
        Label label1 = new Label("Set Name:  ");
        hbox.getChildren().add(label1);
        TextField textField1 = new TextField();
        hbox.getChildren().add(textField1);
        vbox.getChildren().add(hbox);
        Button saveButton = new Button("Save");
        vbox.getChildren().add(saveButton);
        Scene scene = new Scene(vbox);
        stage.setScene(scene);
        stage.show();

        saveButton.setOnAction(new EventHandler<ActionEvent>() {
          @Override
          public void handle(ActionEvent arg0) {
            String setName = textField1.getText();
            IndexTreeItem setItem = new IndexTreeItem();
            setItem.setIndexProperties(item.getValue().getIndexProperties());
            setItem.setName(setName);
            setItem.setIndex(false);
            setItem.setSet(hits);
            setItem.setSetQueryText(searchField.getText());

            TreeItem<IndexTreeItem> indexItem = new TreeItem<IndexTreeItem>(setItem);
            item.getChildren().add(indexItem);
            item.setExpanded(true);

            stage.close();
          }
        });

      }
    });

    searchTab.setContent(searchScene);

    tabPane.getTabs().add(searchTab);
  }

  private void switchToSearchTab(TabPane pane, String query, TreeItem<IndexTreeItem> item) {
    // Put the query in quotes so that it is searched as a phrase
    String phraseQueryString = String.join("", "\"", query, "\"");
    if (searchTab != null && query != null && query.trim().length() > 0) {
      pane.getSelectionModel().select(searchTab);
      searchField.setText(phraseQueryString);

      try {
        hits = searchService.search(item.getValue().getIndexProperties().getIndexDetails(),
            item.getValue().getSet(), searchField.getText());
        searchScene.setCenter(createSearchResultsContent(
            item.getValue().getIndexProperties().getIndexDetails(), hits, searchField.getText()));
      } catch (IOException | ParseException e) {
        System.out.println("Error calling query service");
        e.printStackTrace();
      }
    }
  }

  private BorderPane createSearchResultsContent(IndexDetails indexDetails, List<Integer> hits,
      String query) {
    BorderPane searchResultsLayout = new BorderPane();

    VBox resultsNumLayout = new VBox();
    resultsNumLayout.setPadding(new Insets(10, 10, 10, 10));
    String numDocsReturned = String.join(" ", String.valueOf(hits.size()), "results returned");
    Text resultsText = new Text(numDocsReturned);
    resultsText.setFont(RESULT_NUM_FONT);
    resultsText.setFill(Color.GREY);
    resultsNumLayout.getChildren().add(resultsText);

    searchResultsLayout.setTop(resultsNumLayout);

    int numPages = hits.size() / 10 + 1;
    Pagination pagination = new Pagination(numPages, 0);
    pagination.setPageFactory(new Callback<Integer, Node>() {

      @Override
      public Node call(Integer pageIndex) {
        ScrollPane results = null;
        try {
          results = createSearchLinks(pageIndex.intValue(), hits, indexDetails, query);
        } catch (IOException e) {
          // TODO Auto-generated catch block
          e.printStackTrace();
        }
        return results;
      }
    });

    searchResultsLayout.setCenter(pagination);
    return searchResultsLayout;
  }

  public ScrollPane createSearchLinks(int pageIndex, List<Integer> hits, IndexDetails indexDetails,
      String query) throws IOException {
    ScrollPane scrollPane = new ScrollPane();
    VBox searchLinkLayout = new VBox();
    scrollPane.setContent(searchLinkLayout);
    for (int i = 0; i < 10; i++) {
      String linkNumberString = String.join("", String.valueOf(pageIndex), String.valueOf(i));
      int linkNumber = Integer.valueOf(linkNumberString).intValue();
      if (linkNumber <= hits.size() - 1) {
        DisplayDocument doc = searchService.getDocumentById(indexDetails, hits.get(linkNumber));
        String docText = doc.getFieldsAndContent().get("body");
        Hyperlink link = new Hyperlink(doc.getDocName());
        link.setOnAction(new EventHandler<ActionEvent>() {

          @Override
          public void handle(ActionEvent event) {
            Stage stage = new Stage();
            stage.setTitle("Document: " + doc.getDocName());

            VBox root = new VBox();

            final WebView browser = new WebView();
            final WebEngine webEngine = browser.getEngine();

            ScrollPane scrollPane = new ScrollPane();
            scrollPane.setContent(browser);
            String docContent = createDocText(doc, indexDetails.getSearchableFields(), query);
            webEngine.loadContent(docContent);
            // highlight(webEngine, query);

            root.getChildren().addAll(scrollPane);

            Scene scene = new Scene(root);
            stage.setScene(scene);

            stage.show();
          }

        });
        searchLinkLayout.getChildren().add(link);
        int minLength = Math.min(docText.length(), 200);
        String docAbstract = docText.substring(0, minLength) + "...";
        Text textAbstract = new Text(docAbstract);
        textAbstract.setWrappingWidth(700);
        searchLinkLayout.getChildren().add(textAbstract);
      }
    }

    return scrollPane;
  }

  private String createDocText(DisplayDocument doc, List<String> fields, String query) {
    String jQueryURL = getClass().getResource("/jquery-3.1.1.min.js").toExternalForm();
    String jQueryHighlightURL = getClass().getResource("/jquery-highlight.js").toExternalForm();
    String textminingJsURL = getClass().getResource("/textmining.js").toExternalForm();
    StringJoiner contentBuffer = new StringJoiner("");
    contentBuffer.add("<head>\n");
    contentBuffer.add("<script src=\"" + jQueryURL + "\"></script>\n");
    contentBuffer.add("<script src=\"" + jQueryHighlightURL + "\"></script>\n");
    contentBuffer.add("<script src=\"" + textminingJsURL + "\"></script>\n");
    contentBuffer.add("<style>.highlight { background-color: yellow }</style>");
    contentBuffer.add("</head>");
    contentBuffer.add("<body onload=\"searchAndHighlight();\">");
    contentBuffer.add("<div align=\"right\">");
    contentBuffer
        .add("<input type=\"text\" id=\"search\" value=\"" + query.replace("\"", "") + "\">");
    contentBuffer.add("<button type=\"button\" onclick=\"searchAndHighlight();\">Find</button>");
    contentBuffer.add("</div>");
    for (String field : fields) {
      if (field != null && doc.getFieldsAndContent().get(field) != null) {
        String fieldText = doc.getFieldsAndContent().get(field);
        // fieldText = fieldText.replace("\n", "<br/>");
        // fieldText = fieldText.replace("\r", "<br/>");
        fieldText = fieldText.replace("\t", "<br>&nbsp;&nbsp;&nbsp;&nbsp;");
        fieldText = fieldText.replace("   ", "<br>&nbsp;&nbsp;&nbsp;&nbsp;");
        contentBuffer.add(String.join(" ", "<h3>", field, "</h3>", fieldText, "<br>"));
      }
    }
    contentBuffer.add("</body>");
    String htmlDoc = String.join("", "<html>", contentBuffer.toString(), "</html>");
    return htmlDoc;
  }

  private void createFeatureVectorTab(TabPane tabPane, TreeItem<IndexTreeItem> item,
      Stage primaryStage) {
    Tab fvTab = new Tab();
    fvTab.setText("Feature Vectors");
    fvTab.setClosable(false);

    HBox scene = new HBox();
    VBox inputScene = new VBox();
    inputScene.setPadding(new Insets(10, 10, 10, 10));
    inputScene.setSpacing(20);
    GridPane inputGrid = new GridPane();
    inputGrid.setVgap(5);

    Label labelType = new Label("Label Type:  ");
    labelType.setFont(LABEL_FONT);
    Tooltip labelTooltip = new Tooltip();
    labelTooltip.setText("The category for classification");
    Tooltip.install(labelType, labelTooltip);
    inputGrid.add(labelType, 0, 0);
    ComboBox<String> labelCombo = new ComboBox<>();
    Tooltip.install(labelCombo, labelTooltip);
    List<String> labelFields =
        item.getValue().getIndexProperties().getIndexDetails().getLabelFields();
    if (labelFields != null && labelFields.size() > 0) {
      labelCombo.getItems().addAll(labelFields);
      labelCombo.getSelectionModel().select(labelFields.get(0));
    }
    inputGrid.add(labelCombo, 1, 0);

    Label entityLabel = new Label("Select feature types:  ");
    entityLabel.setFont(LABEL_FONT);
    Tooltip featureTooltip = new Tooltip();
    featureTooltip.setText("The type of features to include in the feature vectors");
    Tooltip.install(entityLabel, featureTooltip);
    inputGrid.add(entityLabel, 0, 1);

    List<CheckBox> featureTypes = new ArrayList<>();

    CheckBox featureType = new CheckBox("term");
    inputGrid.add(featureType, 0, 2);
    featureTypes.add(featureType);

    int rowNum = 3;
    List<String> annotationTypes =
        item.getValue().getIndexProperties().getIndexDetails().getAnnotationTypes();
    if (annotationTypes != null) {
      for (String annotationType : annotationTypes) {
        featureType = new CheckBox(annotationType);
        inputGrid.add(featureType, 0, rowNum);
        featureTypes.add(featureType);
        rowNum++;
      }
    }
    rowNum++;

    Label minFreqLabel = new Label("Minimum Frequency:  ");
    minFreqLabel.setFont(LABEL_FONT);
    Tooltip freqTooltip = new Tooltip();
    freqTooltip.setText("The minimum number of times a feature\n should appear in the corpus");
    Tooltip.install(minFreqLabel, freqTooltip);
    inputGrid.add(minFreqLabel, 0, rowNum);
    TextField minFreq = new TextField();
    Tooltip.install(minFreq, featureTooltip);
    inputGrid.add(minFreq, 1, rowNum);
    rowNum++;
    rowNum++;

    Button submit = new Button("View Features");
    inputGrid.add(submit, 0, rowNum);
    rowNum++;

    inputScene.getChildren().add(inputGrid);
    scene.getChildren().add(inputScene);

    // Results Layout

    BorderPane resultsLayout = new BorderPane();
    resultsLayout.setVisible(false);

    // Set up tables for all labels
    List<String> labels = null;
    try {
      labels = featureVectorService.getLabelValues(
          item.getValue().getIndexProperties().getIndexDetails(), item.getValue().getSet(),
          labelCombo.getValue());
    } catch (IOException e1) {
      System.out.println("Could not get labels for dataset: " + item.getValue().getName());
    }

    Map<String, TableView<Feature>> featureTables = new HashMap<>();
    if (labels != null) {
      TabPane featureTabs = new TabPane();
      for (String label : labels) {
        Tab labelTab = new Tab(label);
        labelTab.setClosable(false);
        TableView<Feature> table = new TableView<>();
        table.setMinWidth(410);
        TableColumn<Feature, String> termColumn = new TableColumn<Feature, String>("Feature");
        termColumn.setCellValueFactory(new PropertyValueFactory<Feature, String>("term"));
        table.getColumns().add(termColumn);
        TableColumn<Feature, String> typeColumn = new TableColumn<Feature, String>("Type");
        typeColumn.setCellValueFactory(new PropertyValueFactory<Feature, String>("type"));
        table.getColumns().add(typeColumn);
        TableColumn<Feature, String> scoreColumn = new TableColumn<Feature, String>("Score");
        scoreColumn.setCellValueFactory(new PropertyValueFactory<Feature, String>("score"));
        table.getColumns().add(scoreColumn);
        labelTab.setContent(table);
        featureTables.put(label, table);
        featureTabs.getTabs().add(labelTab);
      }
      resultsLayout.setCenter(featureTabs);
    }

    VBox bottomLayout = new VBox();

    VBox selectFeaturesLayout = new VBox();
    CheckBox selectAllFeatures = new CheckBox("Select all features");
    selectFeaturesLayout.getChildren().add(selectAllFeatures);
    HBox numFeaturesLayout = new HBox();
    Label numFeaturesLabel = new Label("Number of features to select: ");
    Tooltip numFeaturesTooltip = new Tooltip();
    numFeaturesTooltip
        .setText("The number of features with the top scores\n to select for each category.");
    Tooltip.install(minFreqLabel, freqTooltip);
    numFeaturesLayout.getChildren().add(numFeaturesLabel);
    TextField numFeaturesField = new TextField();
    numFeaturesLayout.getChildren().add(numFeaturesField);
    selectFeaturesLayout.getChildren().add(numFeaturesLayout);
    bottomLayout.getChildren().add(selectFeaturesLayout);

    selectAllFeatures.selectedProperty().addListener(new ChangeListener<Boolean>() {
      @Override
      public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue,
          Boolean newValue) {
        numFeaturesField.setDisable(newValue);
        if (newValue) {
          numFeaturesField.setText("ALL");
        } else {
          numFeaturesField.setText("");
        }
      }
    });

    Button saveResults = new Button("Save Results");
    bottomLayout.getChildren().add(saveResults);
    resultsLayout.setBottom(bottomLayout);

    scene.getChildren().add(resultsLayout);

    if (labels != null) {
      submit.setOnAction(new EventHandler<ActionEvent>() {

        @Override
        public void handle(ActionEvent event) {
          // Remove data from existing tables
          featureTables.forEach((label, table) -> {
            table.getItems().clear();
          });
          // Remove existing warnings
          if (inputScene.getChildren().size() > 1) {
            inputScene.getChildren().remove(1);
          }
          ProgressForm pForm = new ProgressForm(primaryStage);
          List<String> selectedFeatureTypes = new ArrayList<>();
          for (CheckBox featureCheckBox : featureTypes) {
            if (featureCheckBox.isSelected()) {
              selectedFeatureTypes.add(featureCheckBox.getText());
            }
          }

          Task<Void> task = new Task<Void>() {
            @Override
            public Void call() throws InterruptedException {

              if (selectedFeatureTypes.size() > 0 && minFreq.getText() != null
                  && validateInputIsInteger(minFreq.getText())) {
                int minFreqNum = Integer.valueOf(minFreq.getText()).intValue();
                try {
                  featureVectors = featureVectorService.getFeatureVectors(
                      item.getValue().getIndexProperties().getIndexDetails(),
                      item.getValue().getSet(), "class", selectedFeatureTypes, minFreqNum);
                } catch (IOException | ParseException e) {
                  e.printStackTrace();
                }
              }
              return null;
            }
          };
          // binds progress of progress bars to progress of task:
          pForm.activateProgressBar(task);

          // in real life this method would get the result of the task
          // and update the UI based on its value:
          task.setOnSucceeded(event2 -> {
            pForm.getDialogStage().close();
            submit.setDisable(false);
            if (selectedFeatureTypes.size() > 0 && minFreq.getText() != null
                && validateInputIsInteger(minFreq.getText())) {
              featureVectors.forEach((label, featureList) -> {
                TableView<Feature> table = featureTables.get(label);
                // Remove existing table
                scene.getChildren().remove(table);
                table.getItems().clear();

                table.getItems().addAll(featureList);
                table.refresh();
              });
              saveResults.setDisable(false);
              resultsLayout.setVisible(true);
            } else {
              StringJoiner warningJoiner = new StringJoiner("\n");
              if (selectedFeatureTypes.size() < 1) {
                warningJoiner.add("Select at least 1 feature type");
              }
              if (minFreq == null || !validateInputIsInteger(minFreq.getText())) {
                warningJoiner.add("Minimum Frequency must be and integer greater than 1");
              }
              // createWarningsPopup(warningJoiner.toString(), primaryStage);
              Text textWarning = new Text(warningJoiner.toString());
              textWarning.setFont(WARNING_FONT);
              textWarning.setFill(Color.DARKRED);
              inputScene.getChildren().add(textWarning);
              saveResults.setDisable(true);
            }
          });

          task.setOnCancelled(event3 -> {
            saveResults.setDisable(false);
            resultsLayout.setVisible(true);
          });

          task.setOnFailed(event3 -> {
            saveResults.setDisable(false);
            resultsLayout.setVisible(true);
          });

          submit.setDisable(true);
          pForm.getDialogStage().show();

          Thread thread = new Thread(task);
          thread.start();
        }

      });
    }

    saveResults.setOnAction(new EventHandler<ActionEvent>() {

      ProgressForm pForm = new ProgressForm(primaryStage);

      @Override
      public void handle(ActionEvent event) {

        if (selectAllFeatures.isSelected() || (numFeaturesField.getText() != null
            && validateInputIsInteger(numFeaturesField.getText()))) {
          FileChooser fileChooser = new FileChooser();
          fileChooser.getExtensionFilters().add(new ExtensionFilter("ARFF", "*.arff"));
          fileChooser.setTitle("Save Results");
          final File resultsFile = fileChooser.showSaveDialog(primaryStage);
          if (resultsFile != null) {
            Task<Void> task = new Task<Void>() {
              @Override
              public Void call() throws InterruptedException {
                try {
                  int selectedFeatures = 0;
                  if (selectAllFeatures.isSelected()) {
                    selectedFeatures = -1;
                  } else {
                    selectedFeatures = Integer.valueOf(numFeaturesField.getText()).intValue();
                  }

                  featureVectorService.writeARFF(
                      item.getValue().getIndexProperties().getIndexDetails(),
                      item.getValue().getSet(), labelCombo.getValue(), featureVectors,
                      selectedFeatures, resultsFile.getAbsolutePath());

                } catch (NumberFormatException | IOException | ParseException e) {
                  System.out.println("Error creating ARFF file");
                }

                return null;
              }
            };
            // binds progress of progress bars to progress of task:
            pForm.activateProgressBar(task);

            // in real life this method would get the result of the task
            // and update the UI based on its value:
            task.setOnSucceeded(event2 -> {
              pForm.getDialogStage().close();
              saveResults.setDisable(false);
            });

            task.setOnCancelled(event3 -> {
              saveResults.setDisable(false);
              resultsLayout.setVisible(true);
            });

            task.setOnFailed(event3 -> {
              saveResults.setDisable(false);
              resultsLayout.setVisible(true);
            });

            saveResults.setDisable(true);
            pForm.getDialogStage().show();

            Thread thread = new Thread(task);
            thread.start();
          }
        } else {
          createWarningsPopup(
              "Select all features OR\nNumber of features to select must be an integer greater than 1",
              primaryStage);
        }
      }
    });


    fvTab.setContent(scene);

    tabPane.getTabs().add(fvTab);

  }

  private boolean validateInputIsInteger(String input) {
    if (input.matches("[0-9]+")) {
      Integer i = Integer.valueOf(input);
      if (i > 0) {
        return true;
      }
    }
    return false;
  }

  public void createWarningsPopup(String warning, Stage primaryStage) {
    final Stage dialog = new Stage();
    dialog.setX(primaryStage.getX() + 300);
    dialog.setY(primaryStage.getY() + 300);
    dialog.initStyle(StageStyle.UTILITY);
    dialog.initOwner(primaryStage);
    VBox dialogVbox = new VBox();
    dialogVbox.setPadding(new Insets(10, 10, 10, 10));
    Text textWarning = new Text(warning);
    textWarning.setFont(WARNING_FONT);
    textWarning.setFill(Color.DARKRED);
    dialogVbox.getChildren().add(textWarning);
    Scene dialogScene = new Scene(dialogVbox);
    dialog.setScene(dialogScene);
    dialog.sizeToScene();
    dialog.show();
  }

  private void highlight(WebEngine engine, String text) {
    engine.executeScript("$('body').highlight('rat')");
  }

  private void removeHighlight(WebEngine engine) {
    engine.executeScript("$('body').removeHighlight()");
  }

}
