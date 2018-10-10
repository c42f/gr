#if defined(NO_QT5)
#define NO_QT
#endif

#ifndef NO_QT

#include <QtGlobal>
#if QT_VERSION >= 0x050000
    #include <QtWidgets/QWidget>
#else
    #include <QtGui/QWidget>
#endif
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QIcon>

#endif

#define QT_NAME_STRING "Qt5"
#define QT_PLUGIN_ENTRY_NAME gksqt

#include "qtplugin_impl.cxx"

#include "gkswidget.h"

static
void create_pixmap(ws_state_list *p)
{
  p->pm = new QPixmap(p->width, p->height);
  p->pm->fill(Qt::white);

  p->pixmap = new QPainter(p->pm);
  p->pixmap->setClipRect(0, 0, p->width, p->height);

  get_pixmap();
}

static
void resize_pixmap(int width, int height)
{
  if (p->width != width || p->height != height)
    {
      p->width = width;
      p->height = height;

      if (p->pm)
        {
          delete p->pixmap;
          delete p->pm;

          p->pm = new QPixmap(p->width, p->height);
          p->pm->fill(Qt::white);

          p->pixmap = new QPainter(p->pm);
          p->pixmap->setClipRect(0, 0, p->width, p->height);
        }
      p->has_been_resized = 1;
    }
}

GKSWidget::GKSWidget(QWidget *parent)
  : QWidget(parent)
{
  is_mapped = 0;
  dl = NULL;

  gkss->fontfile = gks_open_font();

  p->width = this->width();
  p->height = this->height();
  p->mwidth = this->widthMM() * 0.001;
  p->mheight = this->heightMM() * 0.001;

  initialize_data();

  setWindowTitle(tr("GKS QtTerm"));
  setWindowIcon(QIcon(":/images/gksqt.png"));
}

void GKSWidget::paintEvent(QPaintEvent *)
{

  if (dl)
    {
      QPainter painter(this);
      p->pm->fill(Qt::white);
      interp(dl);
      painter.drawPixmap(0, 0, *(p->pm));
    }
}

void GKSWidget::resizeEvent(QResizeEvent * /*event*/)
{
  p->mwidth = this->widthMM() * 0.001;
  p->mheight = this->heightMM() * 0.001;
  resize_pixmap(this->size().width(), this->size().height());
  repaint();
}

static
int find_ws_viewport_size(char *s, double* vpwidth, double* vpheight)
{
  int sp = 0, *len, *f;
  double *vp;
  len = (int *) (s + sp);
  while (*len)
    {
      f = (int *) (s + sp + sizeof(int));
      if (*f == 55)
        {
          vp = (double *) (s + sp + 3 * sizeof(int));
          *vpwidth = vp[1] - vp[0];
          *vpheight = vp[3] - vp[2];
          return 1;
        }
      sp += *len;
      len = (int *) (s + sp);
    }
  return 0;
}

void GKSWidget::interpret(char *dl)
{
  double vpwidth, vpheight; // desired workspace size in meters
  if (find_ws_viewport_size(dl, &vpwidth, &vpheight))
    {
      int newwidth = nint(vpwidth * this->width()/(this->widthMM()*0.001));
      int newheight = nint(vpheight * this->height()/(this->heightMM()*0.001));
      // Note that resize() may not result in the desired size, depending on
      // the windowing system (especially tiling WMs, but also if the new size
      // is larger than some system defined maximum, etc).
      resize(newwidth, newheight);
    }

  if (!is_mapped)
    {
      is_mapped = 1;
      create_pixmap(p);
      show();
    }

  this->dl = dl;
  repaint();
}
