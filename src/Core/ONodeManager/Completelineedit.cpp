#include "completelineedit.h"
#include <QKeyEvent>
#include <QListView>
#include <QStringListModel>
#include <QDebug>
#include "QListView"
#include "QToolButton"
#include "QMenu"
#include "QAction"
#include "QPainter"

CompleteLineEdit::CompleteLineEdit(QWidget *parent)
    : QLineEdit(parent)
	, m_matchMode(eContains)
	, m_searchButton( new QToolButton(this) )
	, m_optionMenu(nullptr)
	, m_actionMatchMode(nullptr)
	, m_actionCaseSensitive(nullptr)
	, m_caseSensitive(Qt::CaseInsensitive)
{
	Init(m_caseSensitive, m_matchMode);
}

CompleteLineEdit::CompleteLineEdit(bool enableCaseSensitive, bool matchMode, QWidget* parent /* = 0 */)
	: QLineEdit(parent)
	, m_matchMode(eContains)
	, m_searchButton( new QToolButton(this) )
	, m_optionMenu(nullptr)
	, m_actionMatchMode(nullptr)
	, m_actionCaseSensitive(nullptr)
	, m_caseSensitive(Qt::CaseInsensitive)
{
	Init(m_caseSensitive, m_matchMode);
}

CompleteLineEdit::~CompleteLineEdit()
{
	delete m_actionCaseSensitive;
	delete m_actionMatchMode;
	delete m_optionMenu;
	delete m_searchButton;
}

void CompleteLineEdit::InitModelView()
{
	listView = new QListView(this);
	model = new QStringListModel(this);
	listView->setWindowFlags(Qt::ToolTip);

	connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(SetCompleter(const QString &)));
	connect(listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(CompleteText(const QModelIndex &)));
}

QMenu* CompleteLineEdit::CreateOptionMenu(bool enableCaseSensitive, bool matchMode)
{
	QMenu* optionMenu = new QMenu(this);

	m_actionCaseSensitive = new QAction(("���ִ�Сд"), this);
	m_actionCaseSensitive->setCheckable(true);
	m_actionCaseSensitive->setChecked(enableCaseSensitive);
	connect( m_actionCaseSensitive, SIGNAL( toggled(bool) ), this, SLOT( ChangeCaseSensitive(bool) ) );
	optionMenu->addAction(m_actionCaseSensitive);

	m_actionMatchMode = new QAction(("����ƥ��"), this);
	m_actionMatchMode->setCheckable(true);
	m_actionMatchMode->setChecked(matchMode);
	connect( m_actionMatchMode, SIGNAL( toggled(bool) ), this, SLOT( ChangeMatchMode(bool) ) );
	optionMenu->addAction(m_actionMatchMode);

	return optionMenu;
}

void CompleteLineEdit::SetInactiveText(const QString& text)
{
	m_inactiveText = text;
}

void CompleteLineEdit::ChangeCaseSensitive(bool checked)
{
	if (checked)
	{
		m_caseSensitive = Qt::CaseSensitive;
	}
	else
	{
		m_caseSensitive = Qt::CaseInsensitive;
	}
}

void CompleteLineEdit::ChangeMatchMode(bool checked)
{
	if (checked)
	{
		m_matchMode = eContains;
	}
	else
	{
		m_matchMode = eStartWith;
	}
}

void CompleteLineEdit::Init(bool enableCaseSensitive, bool enableWholeWords)
{
	//! ��ʼ��ģ����ͼ
	InitModelView();

	//! ��ʼ����ť�Ͳ˵�
	m_optionMenu = CreateOptionMenu(enableCaseSensitive, enableWholeWords);

	QPixmap pixmap(":/search/Resources/treeview/search.png");
	m_searchButton->setIcon( QIcon(pixmap) );
	m_searchButton->setIconSize( pixmap.size() );
	m_searchButton->setCursor(Qt::ArrowCursor);

	m_searchButton->setStyleSheet("QToolButton { border: none; padding: 0px; margin-left: 1px; }");
	setTextMargins(pixmap.width() + 3, 0, 0, 0);

	SetInactiveText( ("<Tool Name>") );

	connect( m_searchButton, SIGNAL( clicked() ), this, SLOT( ShowOptionMenu() ) );
}

void CompleteLineEdit::ShowOptionMenu()
{
	Q_ASSERT(m_optionMenu);

	m_optionMenu->exec( QCursor::pos() );
}

void CompleteLineEdit::focusInEvent(QFocusEvent* event)
{
    QLineEdit::focusInEvent(event);

    if (displayText().length() > 0)
        selectAll();
}

void CompleteLineEdit::focusOutEvent(QFocusEvent *e) 
{
	QLineEdit::focusOutEvent(e);
	//listView->hide();
}

void CompleteLineEdit::keyPressEvent(QKeyEvent *e) 
{
    if (!listView->isHidden()) 
	{
        int key = e->key();
        int count = listView->model()->rowCount();
        QModelIndex currentIndex = listView->currentIndex();
        if (Qt::Key_Down == key) 
		{
            // �����·����ʱ���ƶ����ѡ����һ������б��е���
            int row = currentIndex.row() + 1;
            if (row >= count) {
                row = 0;
             }
            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        } 
		else if (Qt::Key_Up == key) 
		{
            // �����·����ʱ���ƶ����ѡ����һ������б��е���
            int row = currentIndex.row() - 1;
            if (row < 0) {
                 row = count - 1;
            }
            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        } 
		else if (Qt::Key_Escape == key) 
		{
            // ����Esc��ʱ����������б�
            listView->hide();
        } 
		else if (Qt::Key_Enter == key || Qt::Key_Return == key) 
		{
            // ���»س���ʱ��ʹ������б���ѡ�е������������б�
            if (currentIndex.isValid()) 
			
			{
                QString text = listView->currentIndex().data().toString();
                setText(text);
            }
            listView->hide();
        } 
		else if (Qt::Key_Delete == key) 
		{
            QModelIndexList indexList = listView->selectionModel()->selectedRows();
            QModelIndex index;
            QString str;
            int i = 0;
            foreach(index, indexList) {
                str = index.data().toString();
                this->model->removeRow(index.row() - i);
                word_list.removeAll(str);
                ++i;
            }
        } 
		else 
		{
        // �����������������б���ʹ��QLineEdit�ļ��̰����¼�
            listView->hide();
            QLineEdit::keyPressEvent(e);
        }
    } 
	else 
	{
        QLineEdit::keyPressEvent(e);
    }
}

void CompleteLineEdit::SetCompleter(const QString &text) 
{
    if (text.isEmpty()) {
        listView->hide();
        return;
     }
    //if ((text.length() > 1) && (!listView->isHidden()))  return;

    // �������������б��е�ĳ�����ʰ���������ı��������Ҫ��ʾ������б���
    QStringList tempStr;
    if (m_matchMode == eContains)
    {
		foreach(QString word, word_list) 
		{
			if (word.contains(text, m_caseSensitive)) 
			{
				tempStr << word;
			}
		}
    }
	else
	{
		foreach(QString word, word_list) 
		{
			if (word.startsWith(text, m_caseSensitive)) 
			{
				tempStr << word;
			}
		}
	}
    model->setStringList(tempStr);
    listView->setModel(model);
    if (model->rowCount() == 0) 
	{
      return;
    }
    // Position the text edit
    listView->setMinimumWidth(width());
    listView->setMaximumWidth(width());
    listView->setMaximumHeight(height() *5);
    QPoint p(0, height());
    int x = mapToGlobal(p).x();
    int y = mapToGlobal(p).y();
    listView->move(x, y);
    listView->show();
}

void CompleteLineEdit::CompleteText(const QModelIndex &index) 
{
    QString text = index.data().toString();
    setText(text);
    listView->hide();
}

void CompleteLineEdit::resizeEvent(QResizeEvent *event)
{
  if (!listView->isHidden()) {
    SetCompleter(this->text());
  }
  QLineEdit::resizeEvent(event);

  int y = ( size().height() - m_searchButton->iconSize().height() ) / 2 - 1;

  if (y > 0)
	  m_searchButton->move(m_searchButton->x(), y);
}

void CompleteLineEdit::paintEvent(QPaintEvent* event)
{
	QLineEdit::paintEvent(event);

	if ( text().isEmpty() && !m_inactiveText.isEmpty() && !hasFocus() )
	{
		QPainter painter(this);
		painter.setPen( QColor(Qt::lightGray) );

		int left, top, right, bottom;
		getTextMargins(&left, &top, &right, &bottom);
		painter.drawText(left, top, width(), height(), Qt::AlignLeft | Qt::AlignVCenter, m_inactiveText);
	}
}