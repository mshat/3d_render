/********************************************************************************
** Form generated from reading UI file 'render.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDER_H
#define UI_RENDER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Render
{
public:
    QWidget *widget;
    QLabel *output_canvas1;

    void setupUi(QWidget *Render)
    {
        if (Render->objectName().isEmpty())
            Render->setObjectName(QString::fromUtf8("Render"));
        Render->resize(540, 370);
        widget = new QWidget(Render);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 540, 370));
        output_canvas1 = new QLabel(widget);
        output_canvas1->setObjectName(QString::fromUtf8("output_canvas1"));
        output_canvas1->setGeometry(QRect(0, 0, 540, 370));

        retranslateUi(Render);

        QMetaObject::connectSlotsByName(Render);
    } // setupUi

    void retranslateUi(QWidget *Render)
    {
        Render->setWindowTitle(QApplication::translate("Render", "Render", nullptr));
        output_canvas1->setText(QApplication::translate("Render", "canvas1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Render: public Ui_Render {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDER_H
