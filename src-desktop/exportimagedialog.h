#ifndef EXPORTIMAGEDIALOG_H
#define EXPORTIMAGEDIALOG_H

#include <QDialog>

namespace Ui {
class ExportImageDialog;
}

class ExportImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportImageDialog(QWidget *parent = 0);
    ~ExportImageDialog();

    int getImageWidth() const;
    int getImageHeight() const;

private:
    Ui::ExportImageDialog *_ui;
};

#endif // EXPORTIMAGEDIALOG_H
