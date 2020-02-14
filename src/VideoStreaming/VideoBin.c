#include "VideoBin.h"

#include <stdio.h>
#include <gst/gst.h>
#include <gobject/gobject.h>

static GstBinClass *parent_class;

static gboolean _videoSinkQuery(GstPad* pad, GstObject* parent, GstQuery* query)
{
    GstElement* element = NULL;
    GstQgcSinkBin *bin = GST_QGC_SINK_BIN(parent);

    switch (GST_QUERY_TYPE(query)) {
    case GST_QUERY_CAPS:
        element = bin->glupload;
        break;
    case GST_QUERY_CONTEXT:
        element = bin->qmlglsink;
        break;
    default:
        return gst_pad_query_default (pad, parent, query);
    }

    if (element == NULL) {
        printf("videoSinkQuery(): No element found");
        return FALSE;
    }

    GstPad* sinkpad = gst_element_get_static_pad(element, "sink");

    if (sinkpad == NULL) {
        printf("videoSinkQuery(): No sink pad found");
        return FALSE;
    }

    const gboolean ret = gst_pad_query(sinkpad, query);

    gst_object_unref(sinkpad);
    sinkpad = NULL;

    return ret;
}

GType gst_qgc_sink_bin_get_type (void)
{
  static GType gst_qgc_sink_bin_bin_type = 0;

  if (!gst_qgc_sink_bin_bin_type) {
    static const GTypeInfo gst_qgc_sink_bin_bin_info = {
      .class_size = sizeof (GstQgcSinkBinClass),
      .base_init = NULL,
      .base_finalize = NULL,
      .class_init = (GClassInitFunc) gst_qgc_sink_bin_class_init,
      .class_finalize = NULL,
      .class_data = NULL,
      .instance_size = sizeof (GstQgcSinkBin),
      .n_preallocs = 0,
      .instance_init = (GInstanceInitFunc) gst_qgc_sink_bin_init,
      .value_table = NULL
    };

    gst_qgc_sink_bin_bin_type =
        g_type_register_static (GST_TYPE_BIN, "GstQgcSinkBin",
        &gst_qgc_sink_bin_bin_info, 0);
  }

  return gst_qgc_sink_bin_bin_type;
}

GstElement *gst_qgc_sink_bin_new(const gchar *name)
{
    return gst_element_factory_make("gstqgcsinkbin", name);
}

void gst_qgc_sink_bin_class_init (GstQgcSinkBinClass * klass)
{
  // g_log_set_always_fatal(G_LOG_LEVEL_CRITICAL|G_LOG_LEVEL_WARNING);
  fprintf(stderr, "Calling the init function for the new Bin Class.\n");

  GObjectClass *gobject_klass = (GObjectClass *) klass;
  GstElementClass *gstelement_klass = (GstElementClass *) klass;

  parent_class = g_type_class_peek_parent (klass);

  gobject_klass->dispose = gst_qgc_sink_bin_dispose;
  gobject_klass->finalize = gst_qgc_sink_bin_finalize;
  gobject_klass->set_property = gst_qgc_sink_bin_set_property;
  gobject_klass->get_property = gst_qgc_sink_bin_get_property;

  g_object_class_install_property (gobject_klass, PROP_LAST_SAMPLE,
      g_param_spec_pointer("last-sample", "Last Sample",
          "The last sample of the underline Sink", G_PARAM_READABLE));

  g_object_class_install_property(gobject_klass, PROP_WIDGET,
        g_param_spec_pointer("widget", "Widget", "The OpenGL Container on Qml", G_PARAM_READWRITE));

  gst_element_class_set_static_metadata (gstelement_klass,
      "QGroundControl Sink Bin", "Bin for VideoSink",
      "Bin for VideoSink",
      "Tomaz Canabrava <tcanabrava@kde.org>");

  fprintf(stderr, "Class initialized properly\n");
}

/* Constructor */
void gst_qgc_sink_bin_init(GstQgcSinkBin *bin)
{
    fprintf(stderr, "Calling the init function for the new Bin.\n");
    if ((bin->glupload = gst_element_factory_make("glupload", "glupload")) == NULL) {
       fprintf(stderr, "gst_qgc_sink_bin_init failed. Error with gst_element_factory_make('glupload')\n");
        return;
    }

    if ((bin->glcolorconvert = gst_element_factory_make("glcolorconvert", "glcolorconvert")) == NULL) {
        fprintf(stderr, "gst_qgc_sink_bin_init failed. Error with gst_element_factory_make('glcolorconvert')\n");
        return;
    }

    if ((bin->qmlglsink = gst_element_factory_make("qmlglsink", "qmlglsink")) == NULL) {
        fprintf(stderr, "gst_qgc_sink_bin_init failed. Error with gst_element_factory_make('qmlglsink')\n");
        return;
    }

    GstPad* pad;
    if ((pad = gst_element_get_static_pad(bin->glupload, "sink")) == NULL) {
        fprintf(stderr, "gst_qgc_sink_bin_init failed. Error with gst_element_get_static_pad(glupload, 'sink')\n");
        return;
    }

    gst_bin_add_many(GST_BIN(bin), bin->glupload, bin->glcolorconvert,bin->qmlglsink, NULL);

    gboolean ret = gst_element_link_many(bin->glupload, bin->glcolorconvert, bin->qmlglsink, NULL);

    if (!ret) {
        fprintf(stderr, "gst_qgc_sink_bin_init failed. Error with gst_element_link_many()\n");
        return;
    }

    bin->ghostpad = gst_ghost_pad_new("sink", pad);
    gst_pad_set_query_function(bin->ghostpad, _videoSinkQuery);
    gst_element_add_pad(GST_ELEMENT(bin), bin->ghostpad);

    gst_object_ref(bin->glupload);
    gst_object_ref(bin->qmlglsink);
    gst_object_ref(bin->glcolorconvert);
    gst_object_ref(bin->ghostpad);

    fprintf(stderr, "Object initialized properly\n");
}

/* First part of the destructor */
void gst_qgc_sink_bin_dispose (GObject * object)
{
    g_assert(object);

    GstQgcSinkBin *bin = GST_QGC_SINK_BIN (object);
    g_assert(bin);

    gst_object_unref(bin->glupload);
    gst_object_unref(bin->qmlglsink);
    gst_object_unref(bin->glcolorconvert);
    gst_object_unref(bin->ghostpad);

    bin->glupload = NULL;
    bin->qmlglsink = NULL;
    bin->glcolorconvert = NULL;
    bin->ghostpad = NULL;

    // Parent destructor will actually destroy the objects.
    G_OBJECT_CLASS (parent_class)->dispose (object);
}

/* Second part of the destructor */
void gst_qgc_sink_bin_finalize (GObject * object)
{
  G_OBJECT_CLASS (parent_class)->finalize (object);
}

// forward the "widget" module to the qmlglsink.
void gst_qgc_sink_bin_set_property (GObject * object, guint prop_id, const GValue * value, GParamSpec * pspec)
{
    GstQgcSinkBin *bin = GST_QGC_SINK_BIN (object);
    g_assert(bin);

    switch (prop_id) {
        case PROP_WIDGET:
            fprintf(stderr, "Trying to set the widget property, %p\n", value);
            g_object_set(bin->qmlglsink, "widget", value, NULL);
        break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

void gst_qgc_sink_bin_get_property (GObject * object, guint prop_id, GValue * value, GParamSpec * pspec)
{
    GstQgcSinkBin *bin = GST_QGC_SINK_BIN (object);
    g_assert(bin);

    // forward the property calls call to the inner elements.
    switch (prop_id) {
        case PROP_WIDGET:
            g_object_get(bin->qmlglsink, "widget", value, NULL);
        break;
        case PROP_LAST_SAMPLE:
            g_object_get(bin->qmlglsink, "last-sample", value, NULL);
        break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}
