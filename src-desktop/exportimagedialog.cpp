#include "exportimagedialog.h"
#include "ui_exportimagedialog.h"

ExportImageDialog::ExportImageDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::ExportImageDialog)
{
    _ui->setupUi(this);
}

ExportImageDialog::~ExportImageDialog()
{
    delete _ui;
}


int ExportImageDialog::getImageWidth() const
{
    return _ui->widthSpinBox->value();
}

int ExportImageDialog::getImageHeight() const
{
    return _ui->heightSpinBox->value();
}
