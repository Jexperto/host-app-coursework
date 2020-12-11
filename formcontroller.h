#ifndef FORMCONTROLLER_H
#define FORMCONTROLLER_H
#include <QObject>
class QQmlApplicationEngine;
class QuestionFormList;
class QFile;

class FormController: public QObject {
    Q_OBJECT
    QQmlApplicationEngine* engine;
    QuestionFormList* questionFormList;
    QObject *appWindow;
    QObject *mainPage;
    QObject *pageView;
    QFile* prevPath;
    //QStringList fileList;
public:

    explicit FormController(QObject *parent = nullptr);
    ~FormController();
public slots:
    void onLoadFormButtonClicked();
    void onFileOpened(QString dir);
    void onCreateFormButtonClicked();
signals:

private:
void runQuizForm();
};

#endif // FORMCONTROLLER_H
