#ifndef VIDEOBIN_H
#define VIDEOBIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gst/gstbin.h>

/* This class is a different beast from the usual classes that we have in QGroundControl
 * It's a GObject based class, in C. That's how GStreamer works, that's how we need to do.
 */

typedef struct _GstQgcSinkBin GstQgcSinkBin;
typedef struct _GstQgcSinkBinClass GstQgcSinkBinClass;

#define GST_TYPE_QGC_SINK_BIN             (gst_qgc_sink_bin_get_type())
#define GST_QGC_SINK_BIN_CAST(obj)        ((GstQgcSinkBin*)(obj))
#define GST_QGC_SINK_BIN(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_QGC_SINK_BIN,GstQgcSinkBin))
#define GST_QGC_SINK_BIN_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_QGC_SINK_BIN,GstQgcSinkBinClass))
#define GST_IS_QGC_SINK_BIN(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_QGC_SINK_BIN))
#define GST_IS_QGC_SINK_BIN_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_QGC_SINK_BIN))

/**
 *  GstDecodeBin:
 *
 *  The opaque #GstDecodeBin data structure
 */
struct _GstQgcSinkBin
{
  GstBin bin;                   /* we extend GstBin */

  GstElement* glupload;
  GstElement* glcolorconvert;
  GstElement* qmlglsink;
  GstPad* ghostpad;
};

struct _GstQgcSinkBinClass
{
  GstBinClass parent_class;
};


// Properties:
/* Properties */
enum
{
  PROP_0,
  PROP_LAST_SAMPLE,
  PROP_WIDGET,
};

/********************************
 * Standard GObject boilerplate *
 ********************************/
/* This creates the element just like the normal Gtk elements, and not as a normal GStreamer gst_element_create calls.
 * the reason for this is that I did not registered this element as a Plugin, not sure it's needed.
 */
GstQgcSinkBin *gst_qgc_sink_bin_new();

void gst_qgc_sink_bin_class_init(GstQgcSinkBinClass *klass);
void gst_qgc_sink_bin_init(GstQgcSinkBin *decode_bin);
void gst_qgc_sink_bin_dispose(GObject *object);
void gst_qgc_sink_bin_finalize(GObject *object);

void gst_qgc_sink_bin_set_property (GObject * object, guint prop_id, const GValue * value, GParamSpec * pspec);
void gst_qgc_sink_bin_get_property (GObject * object, guint prop_id, GValue * value, GParamSpec * pspec);

GType gst_qgc_sink_bin_get_type (void);

#define GST_QGC_SINK_BIN_TYPE gst_qgc_sink_bin_get_type ()

#ifdef __cplusplus
}
#endif

#endif // VIDEOBIN_H
