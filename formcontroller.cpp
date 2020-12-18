#include "formcontroller.h"
#include <questionformmodel.h>
#include <questionformlist.h>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlComponent>
#include <QUrl>
#include <QQmlApplicationEngine>
#include <QJsonDocument>
#include <formlist.h>
#include <QJsonArray>
#include "network/roommanager.h"

FormController::FormController(QObject *parent) : QObject(parent)
{
    qmlRegisterType<QuestionFormModel>("CustomQuestionForm",1,0,"QuestionFormModel");
    qmlRegisterUncreatableType<QuestionFormList>("CustomQuestionForm", 1, 0, "FormList",
                                               QStringLiteral("FormList should not be created in QML"));
    questionFormList = new QuestionFormList(this);
    engine =  new QQmlApplicationEngine(this);
    const QUrl url(QStringLiteral("qrc:/Main.qml"));

    connect(engine, &QQmlApplicationEngine::objectCreated,
            QGuiApplication::instance(), [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine->load(url);
    appWindow = engine->rootObjects().takeAt(0);
    mainPage = appWindow->findChild<QObject*>("mainPage");
    pageView = appWindow->findChild<QObject*>("homePage");
    if (pageView){
        qDebug() << "Nice! A Homepage!";
        connect(pageView, SIGNAL(loadTestClicked()), this, SLOT(onLoadFormButtonClicked()));
        connect(pageView, SIGNAL(createRoomClicked()), this, SLOT(onCreateRoomButtonClicked()));
        connect(pageView, SIGNAL(createTestClicked()), this, SLOT(onCreateTestClicked()));
        connect(pageView, SIGNAL(testEventClicked()), this, SLOT(onTestEventClicked()));

    }

//         QFile jsonFile("form.json");
//                if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
//                qDebug() << "Fuck";
//             }
//        FormList form;
//        QVector<QuestionFormItem> items;
//        items.append({ "Test question",2,2,20,true,QStringList({"1","2","3","4"}) });
//        items.append({ "Test question",1,1,20,true,QStringList({"Алохааааааааааа"}) });
//        form.setObjectsList(items);
//        QJsonArray testObject;
//        form.write(testObject);
//        jsonFile.write(QJsonDocument(testObject).toJson());
//        jsonFile.close();




}

FormController::~FormController()
{
    engine->deleteLater();
    questionFormList->deleteLater();
    appWindow->deleteLater();
    mainPage->deleteLater();
    pageView->deleteLater();
}

void FormController::onLoadFormButtonClicked() {
    prevPath = new QFile("previously_opened");
    bool ok = false;
    QUrl url;
    if (prevPath->open(QIODevice::ReadWrite)){
        qDebug() << "OK";
            url = QUrl(QString(prevPath->readAll()));
            ok = true;
    }


    QObject *object;
    QQmlComponent component(engine,
                            QUrl("qrc:/FileDialogForm.qml"));
    if (ok && url.isLocalFile()){
        object = component.createWithInitialProperties({{"folder", QVariant::fromValue(url.toString())}});
        connect(object, SIGNAL(choseFile(QString)), this, SLOT(onFileOpened(QString)));
        return;
    }
    object = component.create();
    connect(object, SIGNAL(choseFile(QString)), this, SLOT(onFileOpened(QString)));
}

void FormController::onFileOpened(QString dir) {\
    if (prevPath->isWritable()){
    prevPath->write(dir.toUtf8());}
    prevPath->close();
     QFile jsonFile(QUrl(dir).toLocalFile());
     if (!jsonFile.open(QIODevice::ReadOnly)) {
          qDebug() << "Can't open JSON";
          return;
       }
            QByteArray saveData = jsonFile.readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
            FormList formList;
            int questionCount = loadDoc.array().count();
            formList.read(loadDoc.array());
            foreach(const auto form, formList.objectsList()){
                questionFormList->appendItem(form);
            }
            runQuizForm(questionCount);
}

void FormController::onCreateRoomButtonClicked()
{
    RoomManager* rm = new RoomManager("25.62.222.154",80,"1.2","00021fe1-1e29-8710-0000-000400000101",this);
   rm->createRoom("77u/NDU4NjQ1NjQ4NjE4NjE3NDg2MTQ2","pool",2,"1.0");


}

void FormController::onTimerElapsed(int index)
{
    qDebug() << "Elapsed " << index;
}

void FormController::onTestCompletedFormClosed()
{
    mainPage->setProperty("source","qrc:/HomeForm.qml");
    pageView = appWindow->findChild<QObject*>("homePage");
    questionFormList->clear();
    if (pageView){
        qDebug() << "Nice! A Homepage!";
        connect(pageView, SIGNAL(loadTestClicked()), this, SLOT(onLoadFormButtonClicked()));
        connect(pageView, SIGNAL(createTestClicked()), this, SLOT(onCreateFormButtonClicked()));
        connect(pageView, SIGNAL(createTestClicked()), this, SLOT(onCreateTestClicked()));
        connect(pageView, SIGNAL(testEventClicked()), this, SLOT(onTestEventClicked()));

    }
}

void FormController::onTestEnded()
{
        qDebug() << "Ended ";
        mainPage->setProperty("source","qrc:/QuizCompletedForm.qml");
        pageView = appWindow->findChild<QObject*>("quizCompPage");
        if (pageView){
                qDebug() << "Nice! An end page!";
            connect(pageView, SIGNAL(formClosed()), this, SLOT(onTestCompletedFormClosed()));
        }
}

void FormController::onCreateTestClicked()
{
 onTestEnded();
}

void FormController::onTestEventClicked()
{
        mainPage->setProperty("source","qrc:/BarStack.qml");
}

void FormController::runQuizForm(int questionCount)
{
    engine->rootContext()->setContextProperty(QStringLiteral("formList"),questionFormList);
    mainPage->setProperty("source","qrc:/QuizForm.qml");
    pageView = appWindow->findChild<QObject*>("quizPage");
    pageView->setProperty("questionsNum",questionCount);
    if (pageView){
        qDebug() << "Nice! A quizPage!";
        connect(pageView, SIGNAL(timerElapsed(int)), this, SLOT(onTimerElapsed(int)));
        connect(pageView, SIGNAL(testEnded()), this, SLOT(onTestEnded()));
    }
}

//QString question;
//int numOfRows;
//int numOfCols;
//int timer;
//bool timerRunning;
//QStringList answers;
