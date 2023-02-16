#include <QtCore/QMetaObject>
#include <QtCore/QObject>
#include <QtGui/QStandardItemModel>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "wininfo.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 创建窗口
    auto window = QScopedPointer<QWidget>(new QWidget());
    window->setWindowTitle("Window Title");

    // 创建下拉列表
    auto *editBox = new QLineEdit();
    {
        editBox->setFocusPolicy(Qt::StrongFocus);

        QObject::connect(editBox, &QLineEdit::textEdited, [](const QString &text)
                         {
      // 处理文本更改
      qDebug() << "Text changed to:" << text; });
    }

    // 创建按钮
    auto button = new QPushButton("Button Text");

    // 创建垂直布局管理器并添加控件
    auto edit_and_button_layout = new QHBoxLayout();
    {
        edit_and_button_layout->addWidget(editBox);
        edit_and_button_layout->addWidget(button);
        // 设置第一个参数可伸缩，第二个不可伸缩
        edit_and_button_layout->setStretch(0, 1);
        edit_and_button_layout->setSizeConstraint(
            QLayout::SetFixedSize); // 设置布局管理器的大小约束
    }
    QStandardItemModel model;
    QIcon defalut_icon(":/images/icon.png");

    auto all_info = GetAllWindowInfo();
    for (auto &win_info : all_info)
    {
        QList<QStandardItem *> row;
        if (win_info.title.empty())
        {
            continue;
        }
        QString title = QString::fromStdWString(win_info.title);
        QIcon win_icon;
        if (win_info.icon_width == 0 || win_info.icon_height == 0)
        {
            win_icon = defalut_icon;
        }
        else
        {
            win_icon = QPixmap::fromImage(QImage(win_info.icon_data.data(), win_info.icon_width, win_info.icon_height, QImage::Format_ARGB32));
        }
        qDebug() << title;
        row.append(new QStandardItem(win_icon, title));
        model.appendRow(row);
    }
    auto list_view = new QListView();
    {
        list_view->setModel(&model);
    }
    auto scrollView = new QScrollArea;
    {
        scrollView->setWidget(list_view);
    }
    auto list_layout = new QVBoxLayout;
    {
        list_layout->addWidget(list_view);
    }
    auto main_layout = new QVBoxLayout;
    {
        main_layout->addLayout(edit_and_button_layout);
        main_layout->addLayout(list_layout);
        main_layout->setStretch(0, 1);
    }
    // 设置窗口的布局管理器
    window->setLayout(main_layout);
    window->setWindowTitle("Inco Search");
    window->resize(400, 40);
    window->setWindowIcon(defalut_icon);
    // 显示窗口
    window->show();

    // 运行应用程序
    return app.exec();
}