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
        connect(pageView, SIGNAL(createTestClicked()), this, SLOT(onCreateFormButtonClicked()));

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
            formList.read(loadDoc.array());
            foreach(const auto form, formList.objectsList()){
                questionFormList->appendItem(form);
            }
            runQuizForm();
}

void FormController::onCreateFormButtonClicked() {
    onFileOpened("file:///C:/Users/aineb/source/Qt/build-HostApp-Desktop_Qt_5_15_1_MSVC2019_64bit-Debug/testjson.json");
}

void FormController::runQuizForm()
{
    engine->rootContext()->setContextProperty(QStringLiteral("formList"),questionFormList);
    mainPage->setProperty("source","qrc:/QuizForm.qml");
}

//QString question;
//int numOfRows;
//int numOfCols;
//int timer;
//bool timerRunning;
//QStringList answers;
