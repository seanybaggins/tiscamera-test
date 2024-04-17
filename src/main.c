/*
 * Copyright 2019 The Imaging Source Europe GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This example will show you how to set properties for a certain camera */

#include "gst/gstclock.h"
#include "gst/gstelement.h"
#include <tcam-property-1.0.h> /* gobject introspection interface */

#include <Tcam-1.0.h>
#include <gst/gst.h>
#include <stdio.h> /* printf and putchar */
#include <unistd.h>

#include <assert.h>

void print_enum_property(GstElement* source, const char* name)
{
    /* this is only a sample not all properties will be set here */

    GError* err = NULL;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source),
                                                                               name,
                                                                               &err);

    if (err)
    {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_ENUMERATION)
    {
        printf("%s has wrong type. This should not happen.\n", name);
    }
    else
    {
        TcamPropertyEnumeration* property_enum = TCAM_PROPERTY_ENUMERATION(property_base);
        const char* value = tcam_property_enumeration_get_value(property_enum, &err);

        if (err)
        {
            printf("Error while retrieving property: %s\n", err->message);
            g_error_free(err);
            err = NULL;
        }
        else
        {
            printf("%s: %s\n", name, value);
        }
    }
    g_object_unref(property_base);
}

void print_float_property(GstElement* source, const char* name)
{
    /* this is only a sample not all properties will be set here */

    GError* err = NULL;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source),
                                                                               name,
                                                                               &err);

    if (err)
    {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_FLOAT)
    {
        printf("%s has wrong type. This should not happen.\n", name);
    }
    else
    {
        TcamPropertyFloat* property_float = TCAM_PROPERTY_FLOAT(property_base);
        const float value = tcam_property_float_get_value(property_float, &err);

        if (err)
        {
            printf("Error while retrieving property: %s\n", err->message);
            g_error_free(err);
            err = NULL;
        }
        else
        {
            printf("%s: %f\n", name, value);
        }
    }
    g_object_unref(property_base);
}

void set_enum_property(GstElement* source, const char* name, const char* value)
{
    GError* err = NULL;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source),
                                                                               name,
                                                                               &err);

    if (err)
    {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_ENUMERATION)
    {
        printf("%s has wrong type. This should not happen.\n", name);
    }
    else
    {
        TcamPropertyEnumeration* enum_property = TCAM_PROPERTY_ENUMERATION(property_base);

        tcam_property_enumeration_set_value(enum_property, value, &err);

        if (err)
        {
            printf("Error while setting property: %s\n", err->message);
            g_error_free(err);
            err = NULL;
        }
    }
    g_object_unref(property_base);
}

void set_float_property(GstElement* source, const char* name, const float value)
{
    GError* err = NULL;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source),
                                                                               name,
                                                                               &err);

    if (err)
    {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_FLOAT)
    {
        printf("%s has wrong type. This should not happen.\n", name);
    }
    else
    {
        TcamPropertyFloat* float_property = TCAM_PROPERTY_FLOAT(property_base);

        tcam_property_float_set_value(float_property, value, &err);

        if (err)
        {
            printf("Error while setting property: %s\n", err->message);
            g_error_free(err);
            err = NULL;
        }
    }
    g_object_unref(property_base);
}

void get_input(const char *prompt, int min, int max, int *value) {
    int input;
    int status;
    do {
        printf("%s", prompt);
        status = scanf("%d", &input);
        while(getchar() != '\n'); // Clear input buffer

        if (status != 1 || input < min || input > max) {
            printf("Invalid input. Please enter a value between %d and %d.\n", min, max);
        }
    } while (status != 1 || input < min || input > max);

    *value = input;
}

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
    GError* err = NULL;
    GstElement* pipeline =
        gst_parse_launch("tcambin name=source"
                         " ! video/x-raw,format=BGRx,width=640,height=480,framerate=30/1"
                         " ! videorate"
                         " ! video/x-raw,framerate=1/2"
                         " ! identity sync=true"
                         " ! timeoverlay"
                         " ! tee name=t ! queue"
                         "      ! ximagesink"
                         " t. ! queue"
                         "      ! jpegenc"
                         "      ! multifilesink location=images/img_%06d.jpg",
                         &err);


    if (pipeline == NULL)
    {
        printf("Unable to create pipeline: %s\n", err->message);
        g_free(err);
        err = NULL;
        return 1;
    }

    /* create a tcambin to retrieve device information */
    GstElement* source = gst_bin_get_by_name(GST_BIN(pipeline), "source");

    const char* serial = NULL;

    if (serial != NULL)
    {
        GValue val = {};
        g_value_init(&val, G_TYPE_STRING);
        g_value_set_static_string(&val, serial);

        g_object_set_property(G_OBJECT(source), "serial", &val);
    }
    gst_element_set_state(pipeline, GST_STATE_READY);
    set_enum_property(source, "GainAuto", "Off");
    print_enum_property(source, "GainAuto");
    set_enum_property(source, "ExposureAuto", "Off");

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    int exposure = 0;
    int gain = 0;
    set_float_property(source, "Gain", (float)gain);
    char choice = 'a';

    while (choice != 'q') {
        printf("Edit Exposure (e) or Gain (g): ");
        
        int _ret = scanf("%c", &choice);
        while(getchar() != '\n'); // Clear input buffer after reading choice

        switch (choice) {
            case 'e':
            case 'E': // Handle both upper and lower case
                get_input("Enter Exposure 1-1,000,000 us: ", 1, 1000000, &exposure);
                set_float_property(source, "ExposureTime", (float)exposure);
                print_float_property(source, "ExposureTime");
                break;
            case 'g':
            case 'G': // Handle both upper and lower case
                get_input("Enter Gain 0-48 db: ", 0, 48, &gain);
                set_float_property(source, "Gain", (float) gain);
                print_float_property(source, "Gain");
                break;
            case 'q':
            case 'Q':
                printf("Exiting Program\n");
                break;
            default:
                printf("Invalid choice. Please enter 'e' for Exposure or 'g' for Gain.\n");
                break;
        }
    }

    return 0;
}
