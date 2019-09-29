#ifndef OPENVIZMG_COMPLETELINEEDIT_H
#define OPENVIZMG_COMPLETELINEEDIT_H

#include <QLineEdit>
#include <QStringList>

class QListView;
class QStringListModel;
class QModelIndex;
class QToolButton;
class QMenu;
class QAction;

class CompleteLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	enum MatchMode
	{
		eStartWith = 0,
		eContains = 1
	};

	explicit CompleteLineEdit(QWidget *parent = 0);
	CompleteLineEdit(bool enableCaseSensitive, bool matchMode, QWidget* parent = 0);

	~CompleteLineEdit();

	void SetInactiveText(const QString& text);
	void InitModelView();

private:
	void Init(bool enableCaseSensitive, bool enableWholeWords);
	QMenu* CreateOptionMenu(bool enableCaseSensitive, bool matchMode);

protected:
	virtual void keyPressEvent(QKeyEvent *e);
	virtual void focusInEvent(QFocusEvent* event);
	virtual void focusOutEvent(QFocusEvent *e);
	virtual void paintEvent(QPaintEvent* event);
	virtual void resizeEvent(QResizeEvent* event);

signals:

	//! �����������
private slots:
	//! ��ʾѡ����Ʋ˵�
	void ShowOptionMenu();

	//! ���Ĵ�Сд����
	void ChangeCaseSensitive(bool checked);

	//! ����ƥ��ѡ��
	void ChangeMatchMode(bool checked);
	//! �������
public slots:
	void SetCompleter(const QString &text);
	void CompleteText(const QModelIndex &index);

private:
	//! ƥ�������model
	QStringListModel* model;

	//! װ�����
	QToolButton* m_searchButton;
	QMenu* m_optionMenu;
	QAction* m_actionCaseSensitive;
	QAction* m_actionMatchMode;
	QString m_inactiveText;

	//! ѡ�����
	//! ��Сд����
	Qt::CaseSensitivity m_caseSensitive;

	//! ƥ��ģʽ
	MatchMode m_matchMode;
public:
	//! ����������Χ
	QStringList word_list;

	QListView* listView;
};

#endif 
