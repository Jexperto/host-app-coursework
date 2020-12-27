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
#include <sendeventmessage.h>
#include "receiveeventmessage.h"
#include <datamanager.h>
#include <QJsonArray>
#include "network/roommanager.h"
#include "chartmodel.h"
#include <QDateTime>


FormController::FormController(QObject *parent) : QObject(parent)
{
    ipAddress = "176.109.34.38";
    //questionsElapsed = 0;
    currentQuestionUserAnswers = new QMap<QString,QString>;
    users = new QMap<QString,QString>;
    pageNumOfQuest = new QSet<int>;
    qmlRegisterType<QuestionFormModel>("CustomQuestionForm",1,0,"QuestionFormModel");
    qmlRegisterType<ChartModel>("ChartModel",1,0,"ChartModel");
    qmlRegisterUncreatableType<QuestionFormList>("CustomQuestionForm", 1, 0, "FormList",
                                               QStringLiteral("FormList should not be created in QML"));
    chartModel = new ChartModel;
    questionFormList = new QuestionFormList(123,this);
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
    pageView = mainPage;
    openHomeForm();

//         QFile jsonFile("form.json");
//                if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
//                qDebug() << "Fuck";
//             }
//        FormList form;
//        QVector<QuestionFormItem> items;
//        items.append({ "Test question",2,2,20,true,"5",QStringList({"1","2","3","4"})});
//        items.append({ "Test question",1,1,20,true,"Aлохааа",QStringList() });
//        form.setObjectsList(items);
//        QJsonArray testObject;
//        form.write(testObject);
//        jsonFile.write(QJsonDocument(testObject).toJson());
//        jsonFile.close();

//     QFile jsonFile("form.json");
//    if (!jsonFile.open(QIODevice::ReadOnly)) {
//                    qDebug() << "Fuck, can't read for shit";
//               }

//               QByteArray saveData = jsonFile.readAll();
//               QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
//               FormList list;
//               list.read(loadDoc.array());


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


void FormController::onReceivedEvent(ReceiveEventMessage *eventMessage)
{
    QString eventType = eventMessage->getEvent();
    qDebug() << "Event:    Received " << eventType << "event";

   if (eventType == "connect" && !testRunning){
        connect(rm,&RoomManager::receivedUsers,this,&FormController::onReceivedUsers);
        rm->getUsers();
   }
   else
   if (eventType == "disconnect"){
   }
   else
   if (eventType == "unknown"){

   }
   else
   if (eventType == "inputData"){
       QJsonObject ans = eventMessage->data();
       qDebug() << "--------User answer: "<< ans;
       QString value;
       if (ans.contains("value")&& ans["value"].isString())
           value = ans["value"].toString();
       else
       if (ans.contains("answer")&& ans["answer"].isString())
           value = ans["answer"].toString();
       qDebug() << "value: "<< value;
       currentQuestionUserAnswers->insert(eventMessage->sender(),value);
   }
   rm->receiveEvent();

}

void FormController::onReceivedUsers(QMap<QString, QString>* users)
{
    this->users = users;
    qDebug() << *users;
}

void FormController::onRefreshButtonClicked()
{
    rm->receiveEvent();
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
    if (pageView!=nullptr)
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
                pageNumOfQuest->insert(form.wrongAnswers.length()+1);
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
            connect(this,&FormController::allPagesCreated,this,[=](){if (pageView!=nullptr){
                    pageView->setProperty("busyIndicatorVisible",QVariant::fromValue(false));
                    pageView->setProperty("startTestButtonVisible",QVariant::fromValue(true));
                }});
            connect(this,&FormController::pageCreationError,this,[=](){openHomeForm();});
}

void FormController::onStartTestClicked()
{
   // questionsElapsed = 0;
    rm->receiveEvent();

    if (pageView!=nullptr)
      pageView->setProperty("busyIndicator",QVariant::fromValue(true));
//    startLambda = [=]()->void{
//        userAnswerResults.clear();
//        for (auto i = users->begin(), j = users->end(); i!=j;i++){
//              userAnswerResults.append(QPair<QString,int>(i.key(),0));
//        }};
            QObject *context = new QObject(this);
            connect(rm,&RoomManager::noMoreEvents, context, [this,context] {
                userAnswerResults.clear();
                for (auto i = users->begin(), j = users->end(); i!=j;i++){
                      userAnswerResults.append(QPair<QString,int>(i.key(),0));
                }

                QJsonObject out;
                out["id"] = "stylesheet";
                SendEventMessage msg(this);
                msg.setData(out);
                msg.setEvent("stylesheet");
                msg.setReceiver("null");
                msg.setSendTimestamp(QDateTime::currentDateTime().toString(Qt::ISODate));
                msg.write(out);
                rm->sendEvent(out);

              runQuizForm(questionCount);
              context->deleteLater(); // changed

            });



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
    if (questNum<1){
        file.setFileName(":/htmls/main.html");
    }
    else
    if (questNum==1){
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
    testRunning = false;
    rm = new RoomManager(ipAddress,80,"1.2","00021fe1-1e29-8710-0000-000400000101",this);
    rm->createRoom("YWRzYmtoZHNhZmhrdmpsZmFzZHZoamthc2ZkdmhqYXNkdmhqaw==",RoomManager::EventMode::POOL,2,"1.0");

    if (pageView!=nullptr)
    pageView->setProperty("busyIndicator",QVariant::fromValue(true));
    QObject* context = new QObject;
    QObject* failedContext = new QObject;

    connect(rm,&RoomManager::roomCreateFailed,failedContext,[this,context,failedContext]()->void{
          pageView->setProperty("busyIndicator",QVariant::fromValue(false));
          context->deleteLater();
          failedContext->deleteLater();
    });
    connect(rm,&RoomManager::roomCreated,context,[this,context,failedContext]()->void{

        connect(rm,&RoomManager::receivedEvent,this,&FormController::onReceivedEvent);
        connect(rm,&RoomManager::receivedUsers,this,&FormController::onReceivedUsers);
        QFile styleFile(":/htmls/app.css");
        if (styleFile.open(QIODevice::ReadOnly)) {
            QByteArray data = styleFile.readAll();
            rm->addResource("stylesheet",data);
          }
        else
             qDebug() << "Can't open stylesheet file";

        QJsonObject info;
        info["name"] = "main";
        QString data = getHTMLPage(0);
        info["data"] = data;
        rm->addPage(info);

        mainPage->setProperty("source","qrc:/RoomCreatedForm.qml");
        if (pageView!=nullptr)
            disconnect(pageView,nullptr,nullptr,nullptr);
        pageView = appWindow->findChild<QObject*>("roomCreatedPage");
        if (pageView!=nullptr){
            pageView->setProperty("codeText",rm->getRoomCode());
            connect(pageView, SIGNAL(deleteRoomClicked()), this, SLOT(onDeleteRoomButtonClicked()));
            connect(pageView, SIGNAL(loadTestClicked()), this, SLOT(onLoadFormButtonClicked()));
            connect(pageView, SIGNAL(createTestClicked()), this, SLOT(onCreateTestClicked()));
            connect(pageView, SIGNAL(startTestClicked()), this, SLOT(onStartTestClicked()));
            connect(pageView, SIGNAL(refreshClicked()), this, SLOT(onRefreshButtonClicked()));
          //connect(pageView, SIGNAL(testEventClicked()), this, SLOT(onTestEventClicked()));
          //connect(pageView, SIGNAL(createPageClicked()), this, SLOT(onCreatePageButtonClicked()));
          //connect(pageView, SIGNAL(deletePageClicked()), this, SLOT(onDeletePageButtonClicked()));})
}
        context->deleteLater();
        failedContext->deleteLater();

    });
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
        SendEventMessage msg(this);
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
    testRunning = true;
    qDebug() << "Elapsed " << index;
    currentIndex = index;

    currentQuestionUserAnswers->clear();
    rm->receiveEvent();
    QObject *context = new QObject(this);
    connect(rm, &RoomManager::noMoreEvents,context,[=]()->void{
                for (auto i = currentQuestionUserAnswers->begin(), j = currentQuestionUserAnswers->end(); i!=j;i++){
                    for (auto currUser = userAnswerResults.begin();currUser!=userAnswerResults.end() ;currUser++) {
                        auto temp = questionFormList->getItem(index).rightAnswer.toLower();
                        auto val = i.value().toLower();
                        if (val==temp && i.key()==currUser->first){
                            ++currUser->second;
                        }
                    }
                }
                foreach(auto user,userAnswerResults){
                  qDebug() << "-------------User results:" <<user;}

                  QJsonObject dbyid;
                  if (index+1 < questionFormList->size()){
                  int optNum = questionFormList->convertItemToEventDataJson(index+1,dbyid);
                 // qDebug() << "----------------------" << dbyid;
                  QString templ = "qst%1";

                  FormLoadStruct fl;
                  fl.mDataByID = dbyid;
                  fl.mFormName = templ.arg(optNum);
                  QJsonObject out;
                  DataManager::write(&fl,&out);
                  SendEventMessage msg(this);
                  msg.setData(out);
                  msg.setEvent("formLoad");
                  msg.setReceiver("null");
                  msg.setSendTimestamp(QDateTime::currentDateTime().toString(Qt::ISODate));
                  msg.write(out);
                 rm->sendEvent(out);
                 qDebug() << "Request sent: "<< out;
                  }
                  context->deleteLater();
                  if (index>=questionCount-1){
                        onTestEnded();
                  }
            });

}

void FormController::onTestCompletedFormClosed()
{
    disconnect(this, nullptr, nullptr,nullptr);
    if (loadFormObject != nullptr)
        loadFormObject->deleteLater();
    if (rm!=nullptr)
        rm->deleteRoom();
    chartModel->clear();
    onDeleteRoomButtonClicked();
    openHomeForm();


}

void FormController::openHomeForm()
{
    pageCount = 0;
    questionCount = 0;
    mainPage->setProperty("source","qrc:/HomeForm.qml");
    if (pageView!=nullptr)
        disconnect(pageView,nullptr,nullptr,nullptr);
    pageView = appWindow->findChild<QObject*>("homePage");
    questionFormList->clear();
    if (pageView!=nullptr){
        qDebug() << "Nice! A Homepage!";
        connect(pageView, SIGNAL(createRoomClicked()), this, SLOT(onCreateRoomButtonClicked()));
        connect(pageView, SIGNAL(testButtonClicked()), this, SLOT(testMethod()));
    }
}

void FormController::testMethod()
{
        questionCount = 10;
        users->insert("1","Me");
        users->insert("2","You");
        users->insert("3","There");
        users->insert("4","In");
        users->insert("5","Babylon");

        userAnswerResults.append(QPair<QString,int>("1",8));
        userAnswerResults.append(QPair<QString,int>("2",7));
        userAnswerResults.append(QPair<QString,int>("3",6));
        userAnswerResults.append(QPair<QString,int>("5",4));
        userAnswerResults.append(QPair<QString,int>("4",5));

            onTestEnded();
 //  mainPage->setProperty("source","qrc:/QuizCompletedForm.qml");
}

void FormController::onTestEnded()
{

    std::sort(userAnswerResults.begin(),userAnswerResults.end(),[](QPair<QString,int>first,QPair<QString,int>second)->bool{return first.second>second.second;});
    qDebug() << "Sorted array: " << userAnswerResults;
    for (auto res = userAnswerResults.begin();res<userAnswerResults.end();res++ ) {
        qDebug() << "Quest count " <<questionCount;
        if (questionCount > 0)
            chartModel->append(users->value(res->first),(double)res->second/questionCount*100);
            qDebug() << users->value(res->first) << (double)res->second/questionCount*100;
    }
        mainPage->setProperty("source","qrc:/QuizCompletedForm.qml");
        if (pageView!=nullptr)
            disconnect(pageView,nullptr,nullptr,nullptr);
        pageView = appWindow->findChild<QObject*>("quizCompPage");
        if (pageView!=nullptr){
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
      qDebug() << "----------------------" << dbyid;
    QString templ = "qst%1";
    FormLoadStruct fl;
    fl.mDataByID = dbyid;
    fl.mFormName = templ.arg(optNum);
    QJsonObject out;
    DataManager::write(&fl,&out);
    SendEventMessage msg(this);
    msg.setData(out);
    msg.setEvent("formLoad");
    msg.setReceiver("null");
    msg.setSendTimestamp(QDateTime::currentDateTime().toString(Qt::ISODate));
    msg.write(out);
    qDebug() << out;
    rm->sendEvent(out);

    mainPage->setProperty("source","qrc:/QuizForm.qml");
    if (pageView!=nullptr)
        disconnect(pageView,nullptr,nullptr,nullptr);
    pageView = appWindow->findChild<QObject*>("quizPage");
    pageView->setProperty("questionsNum",questionCount);
    if (pageView!=nullptr){
        qDebug() << "Nice! A quizPage!";
        connect(pageView, SIGNAL(timerElapsed(int)), this, SLOT(onTimerElapsed(int)));
        //connect(pageView, SIGNAL(testEnded()), this, SLOT(onTestEnded()));
    }
}

//QString question;
//int numOfRows;
//int numOfCols;
//int timer;
//bool timerRunning;
//QStringList answers;
