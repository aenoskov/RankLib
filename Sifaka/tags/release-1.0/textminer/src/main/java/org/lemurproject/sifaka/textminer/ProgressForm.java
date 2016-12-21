package org.lemurproject.sifaka.textminer;

import javafx.concurrent.Task;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.layout.HBox;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

public class ProgressForm {
  private final Stage dialogStage;
  private final ProgressBar pb = new ProgressBar();
  private final ProgressIndicator pin = new ProgressIndicator();

  public ProgressForm(Stage primaryStage) {
    dialogStage = new Stage();
    dialogStage.setTitle("Progress");
    dialogStage.initStyle(StageStyle.UNDECORATED);
    dialogStage.setResizable(false);
    dialogStage.initModality(Modality.APPLICATION_MODAL);
    dialogStage.setX(primaryStage.getX() + 300);
    dialogStage.setY(primaryStage.getY() + 300);

    // PROGRESS BAR
    final Label label = new Label();
    label.setText("alerto");

    pb.setProgress(-1F);
    pin.setProgress(-1F);

    final HBox hb = new HBox();
    hb.setSpacing(5);
    hb.setAlignment(Pos.CENTER);
    hb.getChildren().addAll(pb, pin);

    Scene scene = new Scene(hb);
    dialogStage.setScene(scene);
  }

  public void activateProgressBar(final Task<?> task) {
    // pb.progressProperty().bind(task.progressProperty());
    // pin.progressProperty().bind(task.progressProperty());
    dialogStage.show();
  }

  public Stage getDialogStage() {
    return dialogStage;
  }
}
