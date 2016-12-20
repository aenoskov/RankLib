package org.lemurproject.sifaka.textminer;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.StringJoiner;

import org.lemurproject.sifaka.textminer.domain.IndexTreeItem;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ContextMenu;
import javafx.scene.control.Label;
import javafx.scene.control.MenuItem;
import javafx.scene.control.TextField;
import javafx.scene.control.TreeCell;
import javafx.scene.control.TreeItem;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public final class IndexTreeCell extends TreeCell<IndexTreeItem> {

  private ContextMenu combineSetsMenu = new ContextMenu();

  public IndexTreeCell() {
    MenuItem combineSetsMenuItem = new MenuItem("Combine Sets");
    combineSetsMenu.getItems().add(combineSetsMenuItem);
    combineSetsMenuItem.setOnAction(new EventHandler<ActionEvent>() {

      @Override
      public void handle(ActionEvent event) {
        Stage stage = new Stage();
        stage.setTitle("Combine Sets");
        VBox vbox = new VBox();
        vbox.setAlignment(Pos.TOP_LEFT);
        ObservableList<TreeItem<IndexTreeItem>> children = getTreeItem().getChildren();
        List<IndexTreeItem> childObjects = new ArrayList<>();
        List<IndexTreeItem> selectedChildren = new ArrayList<>();
        for (int i = 0; i < children.size(); i++) {
          IndexTreeItem child = children.get(i).getValue();
          childObjects.add(child);
          CheckBox checkBox = new CheckBox(child.getName());
          checkBox.selectedProperty().addListener(new ChangeListener<Boolean>() {

            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue,
                Boolean newValue) {
              if (newValue) {
                selectedChildren.add(child);
              } else {
                selectedChildren.remove(child);
              }

            }
          });
          vbox.getChildren().add(checkBox);
        }

        HBox hbox = new HBox();
        hbox.setAlignment(Pos.CENTER);
        Label label1 = new Label("Set Name:  ");
        hbox.getChildren().add(label1);
        TextField textField1 = new TextField();
        hbox.getChildren().add(textField1);
        vbox.getChildren().add(hbox);
        Button saveButton = new Button("Save");

        saveButton.setOnAction(new EventHandler<ActionEvent>() {
          @Override
          public void handle(ActionEvent arg0) {
            StringJoiner combinedSetNameJoiner = new StringJoiner(" AND ");
            Set<Integer> combinedSetDocs = new HashSet<>();
            // Iterate over selected sets to get queryText and combine documents
            for (IndexTreeItem selectedChild : selectedChildren) {
              combinedSetNameJoiner.add(selectedChild.getName());
              combinedSetDocs.addAll(selectedChild.getSet());
            }
            String setName = textField1.getText();
            if (setName == null || setName.trim().length() == 0) {
              setName = combinedSetNameJoiner.toString();
            }
            IndexTreeItem setItem = new IndexTreeItem();
            setItem.setIndexProperties(getItem().getIndexProperties());
            setItem.setName(setName);
            setItem.setIndex(false);
            setItem.setSet(new ArrayList<Integer>(combinedSetDocs));
            setItem.setSetQueryText(combinedSetNameJoiner.toString());

            TreeItem<IndexTreeItem> indexItem = new TreeItem<IndexTreeItem>(setItem);
            getTreeItem().getChildren().add(indexItem);

            stage.close();
          }
        });

        vbox.getChildren().add(saveButton);

        Scene scene = new Scene(vbox);
        stage.setScene(scene);
        stage.show();
      }
    });
  }

  @Override
  public void updateItem(IndexTreeItem item, boolean empty) {
    super.updateItem(item, empty);

    if (empty) {
      setText(null);
    } else {
      setText(getString());
      if (getItem() != null && getItem().isIndex()) {
        setContextMenu(combineSetsMenu);
      }
    }
  }

  private String getString() {
    return getItem() == null ? "" : getItem().toString();
  }

}
