// -*-c++-*- VCity project, 3DUSE, Liris, 2013, 2014
////////////////////////////////////////////////////////////////////////////////
#ifndef DIALOGEDITLAYER_HPP
#define DIALOGEDITLAYER_HPP
////////////////////////////////////////////////////////////////////////////////
#include <QDialog>
#include "core/URI.hpp"
////////////////////////////////////////////////////////////////////////////////
namespace Ui {
class DialogEditLayer;
}
////////////////////////////////////////////////////////////////////////////////
class DialogEditLayer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditLayer(QWidget *parent = 0);
    ~DialogEditLayer();

    void editLayer(const vcity::URI& uri);

    void setName(const QString& str);
    QString getName() const;

private:
    Ui::DialogEditLayer *ui;
};
////////////////////////////////////////////////////////////////////////////////
#endif // DIALOGEDITLAYER_HPP
