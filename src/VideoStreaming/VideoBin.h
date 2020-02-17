#ifndef VIDEOBIN_H
#define VIDEOBIN_H

#include <gst/gstbin.h>

G_BEGIN_DECLS

/* This class is a different beast from the usual classes that we have in QGroundControl
 * It's a GObject based class, in C. That's how GStreamer works, that's how we need to do.
 */

typedef struct _GstQgcSinkBin GstQgcSinkBin;

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
  PROP_LAST_SAMPLE = 1,
  PROP_WIDGET,
  N_PROPS
};

/********************************
 * Standard GObject boilerplate *
 ********************************/
/* This creates the element just like the normal Gtk elements, and not as a normal GStreamer gst_element_create calls.
 * the reason for this is that I did not registered this element as a Plugin, not sure it's needed.
 */


#define GST_QGC_TYPE_SINK_BIN gst_qgc_sink_bin_get_type ()
G_DECLARE_FINAL_TYPE(GstQgcSinkBin, gst_qgc_sink_bin, GST_QGC, SINK_BIN, GstBin)

GstElement *gst_qgc_sink_bin_new();

void gst_qgc_sink_bin_class_init(GstQgcSinkBinClass *klass);
void gst_qgc_sink_bin_init(GstQgcSinkBin *decode_bin);
void gst_qgc_sink_bin_dispose(GObject *object);
void gst_qgc_sink_bin_finalize(GObject *object);

void gst_qgc_sink_bin_set_property (GObject * object, guint prop_id, const GValue * value, GParamSpec * pspec);
void gst_qgc_sink_bin_get_property (GObject * object, guint prop_id, GValue * value, GParamSpec * pspec);

G_END_DECLS

#endif // VIDEOBIN_H
