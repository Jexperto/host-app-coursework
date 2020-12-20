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
#include <eventmessage.h>
#include <datamanager.h>
#include <QJsonArray>
#include "network/roommanager.h"
#include "chartmodel.h"
#include <QDateTime>


FormController::FormController(QObject *parent) : QObject(parent)
{

    pageNumOfQuest = new QSet<int>;
    qmlRegisterType<QuestionFormModel>("CustomQuestionForm",1,0,"QuestionFormModel");
    qmlRegisterType<ChartModel>("ChartModel",1,0,"ChartModel");
    qmlRegisterUncreatableType<QuestionFormList>("CustomQuestionForm", 1, 0, "FormList",
                                               QStringLiteral("FormList should not be created in QML"));
    chartModel = new ChartModel;
    questionFormList = new QuestionFormList(this);
    engine =  new QQmlApplicationEngine(this);
    engine->rootContext()->setContextProperty(QStringLiteral("chartModel"),chartModel);
    //engine->rootContext()->setContextProperty("chartModel", QVariant::fromValue(*chartModel));

    const QUrl url(QStringLiteral("qrc:/Main.qml"));

    connect(engine, &QQmlApplicationEngine::objectCreated,
            QGuiApplication::instance(), [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine->load(url);
    appWindow = engine->rootObjects().takeAt(0);
    mainPage = appWindow->findChild<QObject*>("mainPage");
    openHomeForm();

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
    if (rm!=nullptr)
        rm->deleteRoom();
    delete chartModel; //TODO: clear inside objects
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

    QQmlComponent component(engine,
                            QUrl("qrc:/FileDialogForm.qml"));
    if (ok && url.isLocalFile()){
        loadFormObject = component.createWithInitialProperties({{"folder", QVariant::fromValue(url.toString())}});
        connect(loadFormObject, SIGNAL(choseFile(QString)), this, SLOT(onFileOpened(QString)));
        return;
    }
    loadFormObject = component.create();
    connect(loadFormObject, SIGNAL(choseFile(QString)), this, SLOT(onFileOpened(QString)));
}

void FormController::onFileOpened(QString dir) {
    if (pageView)
    pageView->setProperty("busyIndicatorVisible",QVariant::fromValue(true));

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
            questionCount = loadDoc.array().count();
            formList.read(loadDoc.array());
            pageNumOfQuest->clear();
            foreach(const auto form, formList.objectsList()){
                questionFormList->appendItem(form);
                pageNumOfQuest->insert(form.answers.length());
            }
            if(rm!=nullptr){
                 QString templ = "qst%1";
                  connect(rm,&RoomManager::pageCreated,this,&FormController::onPageCreated);
            foreach(const auto q, *pageNumOfQuest){
                    QJsonObject info;
                    info["name"] = templ.arg(q);
                    QString data = getHTMLPage(q);
                    info["data"] = data;
                    qDebug() << data;
                    rm->addPage(info);
                }
            }
            connect(this,&FormController::allPagesCreated,this,[=](){if (pageView){
                    pageView->setProperty("busyIndicatorVisible",QVariant::fromValue(false));
                    pageView->setProperty("startTestButtonVisible",QVariant::fromValue(true));
                }});
            connect(this,&FormController::pageCreationError,this,[=](){openHomeForm();});
}

void FormController::onStartTestClicked()
{
     runQuizForm(questionCount);
}

void FormController::onPageCreated(bool success)
{
    if (success){
        pageCount++;
        if (pageCount == pageNumOfQuest->size())
            emit allPagesCreated();
   }
    else{
        pageCount = 0;
        emit pageCreationError();
    }
}



QString FormController::getHTMLPage(int questNum)
{
    QFile file;
    if (questNum<2){
        file.setFileName(":/htmls/text_question.html");
    }
    else{
        QString url = ":/htmls/%1opt_question.html";
        file.setFileName(url.arg(questNum));
    }
    if (!file.exists()) {
        qDebug() << "File doesn't exist";
    }
       if (!file.open(QIODevice::ReadOnly))
           return "null";
       return file.readAll();
}


void FormController::onCreateRoomButtonClicked()
{
    rm = new RoomManager("25.62.222.154",80,"1.2","00021fe1-1e29-8710-0000-000400000101",this);
    rm->createRoom("77u/NDU4NjQ1NjQ4NjE4NjE3NDg2MTQ2",RoomManager::EventMode::POOL,2,"1.0");
    if (pageView)
    pageView->setProperty("busyIndicator",QVariant::fromValue(true));
    connect(rm,&RoomManager::roomCreated,this,[=]()->void{
        mainPage->setProperty("source","qrc:/RoomCreatedForm.qml");
        pageView = appWindow->findChild<QObject*>("roomCreatedPage");
        if (pageView){
            pageView->setProperty("codeText",rm->getRoomCode());
          connect(pageView, SIGNAL(deleteRoomClicked()), this, SLOT(onDeleteRoomButtonClicked()));
          connect(pageView, SIGNAL(loadTestClicked()), this, SLOT(onLoadFormButtonClicked()));
          connect(pageView, SIGNAL(createTestClicked()), this, SLOT(onCreateTestClicked()));
            connect(pageView, SIGNAL(startTestClicked()), this, SLOT(onStartTestClicked()));
          //connect(pageView, SIGNAL(testEventClicked()), this, SLOT(onTestEventClicked()));
          //connect(pageView, SIGNAL(createPageClicked()), this, SLOT(onCreatePageButtonClicked()));
          //connect(pageView, SIGNAL(deletePageClicked()), this, SLOT(onDeletePageButtonClicked()));})
}});
}
void FormController::onDeleteRoomButtonClicked()
{
    if (rm!=nullptr){
        rm->deleteRoom();
        openHomeForm();
    }
}

void FormController::onCreatePageButtonClicked()
{
    if(rm!=nullptr){
        QJsonObject info;
        info["name"] = "main";
        info["data"] = "TestPageData";
        rm->addPage(info);
    }
}

void FormController::onDeletePageButtonClicked()
{
    if(rm!=nullptr){
        QJsonObject info;
        info["name"] = "TestPageName";
        rm->deletePage(info);
    }
}

void FormController::onTestEventClicked()
{
    if(rm!=nullptr){
        EventMessage msg(this);
        msg.setEvent("formLoad");
        msg.setReceiver("null");
        msg.setSendTimestamp("2020-12-19T14:37:36");
        QJsonObject obj;
        FormLoadStruct* form = new FormLoadStruct{"TestPageName",QJsonObject(),QJsonObject()};
        DataManager::write(form,&obj);
        msg.setData(obj);
        QJsonObject res;
        msg.write(res);
        qDebug() << res;
        rm->sendEvent(res);
    }

}


void FormController::onTimerElapsed(int index)
{
    QJsonObject dbyid;
    if (index+1 < questionFormList->size()){
    int optNum = questionFormList->convertItemToEventDataJson(index+1,dbyid);
    qDebug() << "Elapsed " << index;
    QString templ = "qst%1";

    FormLoadStruct fl;
    fl.mDataByID = dbyid;
    fl.mFormName = templ.arg(optNum);
    QJsonObject out;
    DataManager::write(&fl,&out);
    EventMessage msg(this);
    msg.setData(out);
    msg.setEvent("formLoad");
    msg.setReceiver("null");
    msg.setSendTimestamp(QDateTime::currentDateTime().toString(Qt::ISODate));
    msg.write(out);
    qDebug() << out;
   rm->sendEvent(out);
    }
}

void FormController::onTestCompletedFormClosed()
{
    if (loadFormObject != nullptr)
        loadFormObject->deleteLater();
    onDeleteRoomButtonClicked();
    openHomeForm();


}

void FormController::openHomeForm()
{
    pageCount = 0;
    questionCount = 0;
    mainPage->setProperty("source","qrc:/HomeForm.qml");
    pageView = appWindow->findChild<QObject*>("homePage");
    questionFormList->clear();
    if (pageView){
        qDebug() << "Nice! A Homepage!";
        connect(pageView, SIGNAL(createRoomClicked()), this, SLOT(onCreateRoomButtonClicked()));
    }
}

void FormController::onTestEnded()
{
    chartModel->append("Peter",100);
    chartModel->append("Lois",70);
    chartModel->append("Meg",5);
        mainPage->setProperty("source","qrc:/QuizCompletedForm.qml");
        pageView = appWindow->findChild<QObject*>("quizCompPage");
        if (pageView){
                qDebug() << "Nice! An end page!";
            connect(pageView, SIGNAL(formClosed()), this, SLOT(onTestCompletedFormClosed()));
        }
}

void FormController::onCreateTestClicked()
{
   //onTestEnded();
   // mainPage->setProperty("source","qrc:/RoomCreatedForm.qml");

}


void FormController::runQuizForm(int questionCount)
{
    engine->rootContext()->setContextProperty(QStringLiteral("formList"),questionFormList);

    QJsonObject dbyid;
    int optNum = questionFormList->convertItemToEventDataJson(0,dbyid);
    QString templ = "qst%1";
    FormLoadStruct fl;
    fl.mDataByID = dbyid;
    fl.mFormName = templ.arg(optNum);
    QJsonObject out;
    DataManager::write(&fl,&out);
    EventMessage msg(this);
    msg.setData(out);
    msg.setEvent("formLoad");
    msg.setReceiver("null");
    msg.setSendTimestamp(QDateTime::currentDateTime().toString(Qt::ISODate));
    msg.write(out);
    qDebug() << out;
    rm->sendEvent(out);

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
