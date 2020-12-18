import QtQuick 2.12
import QtQuick.Controls 2.5
import CustomQuestionForm 1.0
Item {
    id:root
    objectName: "quizPage"
    width: 1280
    height: 720
    visible: true
    property var model
    property int questionsNum: 0
    readonly property color colorMain: "#f4f4f6"
    readonly property color colorAccentBlue: "#234068"
    readonly property color colorAccentRed: "#e63946"
    signal timerElapsed(int index);
  //signal testStarted();
    signal testEnded();

    QtObject{
    id: props
    property int currentIndex: 0
    }
//run first question timer

    SwipeView{
        id: swipeView
        anchors.fill: parent
        interactive: false
        Repeater{
            model: QuestionFormModel{list: formList}
            QuestionForm{
                            id: qform;
                            timer: model.timer;
                            timerRunning: model.timerRunning;
                            question: model.question
                            numOfRows: model.numOfRows;
                            numOfCols:model.numOfCols;
                            answers: model.answers

                            onTimerElapsed: {
                            root.timerElapsed(props.currentIndex)
                            props.currentIndex++;
                            if (props.currentIndex == questionsNum){
                                testEnded();
                                return}
                            swipeRight.trigger();

                            }

                           }
}



//            QuestionForm
//            {numOfRows: 2; numOfCols:2; timerRunning: true; question: "Тестовый вопрос"
//                        model: ListModel{
//                        ListElement {name: "Первый вариант ответа"}
//                        ListElement {name: "Второй вариант ответа"}
//                        ListElement {name: "Третий вариант ответа"}
//                        ListElement {name: "Четвертый вариант ответа"}
//                }
//            }
//            QuestionForm
//            {numOfRows: 2; numOfCols:1; question: "Другой тестовый вопрос"
//                        model: ListModel{
//                        ListElement {name: "Введите ответ в текстовом поле на своём устройстве"}
//                }
//            }
//        QuestionForm
//        {numOfRows: 2; numOfCols:2; timerRunning: false; question: "Что есть величайшая иллюзая жизни?"
//                    model: ListModel{
//                    ListElement {name: "Хмм... Ну, фокус, где пополам женщину режут"}
//                    ListElement {name: "Счастливый брак?"}
//                    ListElement {name: "Невинность, брат мой"}
//                    ListElement {name: "Мечты - это реальность, а реальность - это мечта"}
//            }
//        }

//        QuestionForm{numOfRows: 2; numOfCols: 2; question: "Что есть музыка жизни?"
//                    model: ListModel{
//                    ListElement {name: "Хмм... Лютня? Нет, барабаны!"}
//                    ListElement {name: "Вопль?"}
//                    ListElement {name: "Тишина, брат мой."}
//                    ListElement {name: "Хор, наверно. И песнопения."}
//            }


  }

    Action {
        id: swipeRight
       // shortcut: "Right"
        onTriggered:{if(swipeView.currentIndex<swipeView.count-1){
                swipeView.itemAt(swipeView.currentIndex).timerRunning=false;
                swipeView.currentIndex+=1;
                swipeView.itemAt(swipeView.currentIndex).timerRunning=true;
        }
        }

    }
    Action {
        id: swipeLeft
        //shortcut: "Left"
        onTriggered:{if(swipeView.currentIndex>0) {
                swipeView.itemAt(swipeView.currentIndex).timerRunning=false;
                swipeView.currentIndex-=1;
                swipeView.itemAt(swipeView.currentIndex).timerRunning=true;
        }}

    }

    PageIndicator {
            id: questionPageIndicator
            count: swipeView.count
            currentIndex: swipeView.currentIndex
            interactive: false
            anchors.bottom: swipeView.bottom
            anchors.bottomMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter

            delegate: Rectangle {
                   implicitWidth: 12
                   implicitHeight: 12

                   radius: width / 2
                   color: "white"

                   opacity: index === swipeView.currentIndex ? 0.95 : 0.25

                   Behavior on opacity {
                       OpacityAnimator {
                           duration: 100
                       }
                   }

        }
}

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/
