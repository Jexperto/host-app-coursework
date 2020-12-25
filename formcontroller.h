#ifndef FORMCONTROLLER_H
#define FORMCONTROLLER_H
#include <QObject>

class QQmlApplicationEngine;
class ReceiveEventMessage;
class QuestionFormList;
class QFile;
class RoomManager;
class ChartModel;
class FormController: public QObject {
    Q_OBJECT
    QQmlApplicationEngine* engine;
    bool testRunning;
    QuestionFormList* questionFormList;
    QObject *appWindow;
    QObject *mainPage;
    QObject *pageView;
    QFile* prevPath;
    ChartModel* chartModel;
    QSet<int>* pageNumOfQuest;
    RoomManager* rm;
    int pageCount;
    int questionCount;
    QObject *loadFormObject;
    QMap<QString,QString>* users;
    QList<QPair<QString, int>> userAnswerResults;
    QMap<QString,QString>* currentQuestionUserAnswers;
    std::function<void()> startLambda;
    int currentIndex;
    QString ipAddress;
    //int questionsElapsed;
    //QStringList fileList;
public:
    explicit FormController(QObject *parent = nullptr);
    ~FormController();
public slots:
    void testMethod();
    void onReceivedEvent(ReceiveEventMessage* eventMessage);
    void onReceivedUsers(QMap<QString,QString>* users);
    void onLoadFormButtonClicked();
    void onRefreshButtonClicked();
    void onFileOpened(QString dir);
    void onTimerElapsed(int index);
    void onTestCompletedFormClosed();
    void onTestEnded();
    void onCreateTestClicked();
    void onTestEventClicked();
    void onCreateRoomButtonClicked();
    void onCreatePageButtonClicked();
    void onDeleteRoomButtonClicked();
    void onDeletePageButtonClicked();
    void onPageCreated(bool success);
    void onStartTestClicked();
signals:
void userConnected();
void allPagesCreated();
void pageCreationError();
private:

void runQuizForm(int questionCount);
QString getHTMLPage(int questNum);
void openHomeForm();
};

#endif // FORMCONTROLLER_H
