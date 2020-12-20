#ifndef FORMCONTROLLER_H
#define FORMCONTROLLER_H
#include <QObject>

class QQmlApplicationEngine;
class QuestionFormList;
class QFile;
class RoomManager;
class ChartModel;
class FormController: public QObject {
    Q_OBJECT
    QQmlApplicationEngine* engine;
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
    //QStringList fileList;
public:
    explicit FormController(QObject *parent = nullptr);
    ~FormController();
public slots:
    void onLoadFormButtonClicked();
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
void allPagesCreated();
void pageCreationError();
private:

void runQuizForm(int questionCount);
QString getHTMLPage(int questNum);
void openHomeForm();
};

#endif // FORMCONTROLLER_H
