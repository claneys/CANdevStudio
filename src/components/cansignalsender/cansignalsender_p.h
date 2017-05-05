#ifndef CANSIGNALSENDER_P_H
#define CANSIGNALSENDER_P_H

#include "cansignalsender.h"
#include <QDebug>
#include <QHeaderView>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTableView>
#include <QToolBar>
#include <QVBoxLayout>
#include <memory>

class CanSignalSenderPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CanSignalSender)

public:
    CanSignalSenderPrivate(CanSignalSender* q)
        : q_ptr(q)
    {
    }

    void setupUi(QWidget* w)
    {
        QVBoxLayout* layout = new QVBoxLayout();
        QToolBar* toolbar = new QToolBar();
        QTableView* tv = new QTableView();
        QStandardItemModel* tvModel = new QStandardItemModel(0, 3);
        QPushButton* pbAdd = new QPushButton("Add");

        tvModel->setHorizontalHeaderLabels({ tr("name"), tr("value"), "" });
        tv->setModel(tvModel);
        tv->verticalHeader()->hideSection(0);
        tv->setColumnWidth(0, 260);
        tv->setColumnWidth(1, 120);
        tv->horizontalHeader()->setStretchLastSection(true);

        toolbar->addWidget(pbAdd);

        layout->addWidget(toolbar);
        layout->addWidget(tv);

        connect(pbAdd, &QPushButton::pressed, [this, tv, tvModel]() {
            QStandardItem* name = new QStandardItem();
            QStandardItem* value = new QStandardItem();
            QList<QStandardItem*> list{ name, value };
            tvModel->appendRow(list);
            tv->scrollToBottom();
            QPushButton* pbSend = new QPushButton("Send");
            tv->setIndexWidget(tvModel->index(tvModel->rowCount() - 1, 2), pbSend);

            connect(pbSend, &QPushButton::pressed, [this, name, value]() {
                Q_Q(CanSignalSender);

                if (!started) {
                    qDebug() << "CanDevice not activated";
                    return;
                    return;
                }

                if (name->text().size() && value->text().size()) {
                    quint32 val;
                    if (value->text().startsWith("0x"))
                        val = value->text().toUInt(nullptr, 16);
                    else
                        val = value->text().toUInt(nullptr, 10);

                    QByteArray ba = QByteArray::number(val);
                    emit q->sendSignal(name->text(), ba);
                }
            });
        });

        // Testing code start
        // TODO: remove
        pbAdd->click();
        pbAdd->click();
        pbAdd->click();
        pbAdd->click();
        pbAdd->click();
        pbAdd->click();
        pbAdd->click();
        pbAdd->click();
        pbAdd->click();
        pbAdd->click();
        tvModel->item(0, 0)->setText("vehicle.engine.oilpressure");
        tvModel->item(1, 0)->setText("vehicle.engine.rpm");
        tvModel->item(2, 0)->setText("vehicle.engine.temperature");
        tvModel->item(3, 0)->setText("vehicle.turnsignal.right");
        tvModel->item(4, 0)->setText("vehicle.turnsignal.left");
        tvModel->item(5, 0)->setText("vehicle.battery");
        tvModel->item(6, 0)->setText("vehicle.transmission.gear");
        tvModel->item(7, 0)->setText("vehicle.ignition");
        tvModel->item(8, 0)->setText("vehicle.fuel");
        tvModel->item(9, 0)->setText("vehicle.speed");
        // Testing code end

        w->setLayout(layout);
    }

    bool started{ false };

private:
    CanSignalSender* q_ptr;
};

#endif // CANSIGNALSENDER_P_H
