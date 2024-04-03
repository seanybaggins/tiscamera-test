/* This example will show you how to start a live stream from your camera */


#include <gstreamer-1.0/gst/gst.h>

#include <stdio.h> /* printf and putchar */



int main(int argc, char* argv[])

{

    /* this line sets the gstreamer default logging level

       it can be removed in normal applications

       gstreamer logging can contain verry useful information

       when debugging your application

       # see https://gstreamer.freedesktop.org/documentation/tutorials/basic/debugging-tools.html

       for further details

    */

    gst_debug_set_default_threshold(GST_LEVEL_WARNING);


    gst_init(&argc, &argv); // init gstreamer


    const char* serial = NULL; // set this if you do not want the first found device


    GError* err = NULL;


    GstElement* pipeline =

        gst_parse_launch("tcambin name=source ! videoconvert ! ximagesink sync=false", &err);


    if (err)

    {

        printf("%s\n", err->message);

        g_error_free(err);

        err = NULL;

    }


    /* test for error */

    if (pipeline == NULL)

    {

        printf("Could not create pipeline. Cause: %s\n", err->message);

        return 1;

    }


    if (serial != NULL)

    {

        GstElement* source = gst_bin_get_by_name(GST_BIN(pipeline), "source");

        GValue val = {};

        g_value_init(&val, G_TYPE_STRING);

        g_value_set_static_string(&val, serial);


        g_object_set_property(G_OBJECT(source), "serial", &val);


        gst_object_unref(source);

    }


    gst_element_set_state(pipeline, GST_STATE_PLAYING);


    printf("Press enter to stop the stream.\n");

    getchar();


    gst_element_set_state(pipeline, GST_STATE_NULL);


    gst_object_unref(pipeline);


    return 0;

}
