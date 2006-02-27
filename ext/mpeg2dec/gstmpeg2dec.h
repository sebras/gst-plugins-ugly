/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GST_MPEG2DEC_H__
#define __GST_MPEG2DEC_H__


#include <gst/gst.h>
#include <mpeg2.h>

G_BEGIN_DECLS

#define GST_TYPE_MPEG2DEC \
  (gst_mpeg2dec_get_type())
#define GST_MPEG2DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MPEG2DEC,GstMpeg2dec))
#define GST_MPEG2DEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MPEG2DEC,GstMpeg2dec))
#define GST_IS_MPEG2DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MPEG2DEC))
#define GST_IS_MPEG2DEC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MPEG2DEC))

#define MPEG_TIME_TO_GST_TIME(time) (((time) * (GST_MSECOND/10)) / 9LL)
#define GST_TIME_TO_MPEG_TIME(time) (((time) * 9LL) / (GST_MSECOND/10))

#define GST_MPEG2DEC_NUM_BUFS 6

typedef struct _GstMpeg2dec GstMpeg2dec;
typedef struct _GstMpeg2decClass GstMpeg2decClass;

typedef enum
{
  MPEG2DEC_FORMAT_NONE,
  MPEG2DEC_FORMAT_I422,
  MPEG2DEC_FORMAT_I420,
  MPEG2DEC_FORMAT_YV12
} Mpeg2decFormat;

typedef enum 
{
  MPEG2DEC_DISC_NONE            = 0,
  MPEG2DEC_DISC_NEW_PICTURE,
  MPEG2DEC_DISC_NEW_KEYFRAME
} DiscontState;

struct _GstMpeg2dec {
  GstElement     element;

  /* pads */
  GstPad        *sinkpad,
                *srcpad,
                *userdatapad;

  mpeg2dec_t    *decoder;
  const mpeg2_info_t *info;

  gboolean       closed;
  gboolean       have_fbuf;

  GstBuffer     *buffers[GST_MPEG2DEC_NUM_BUFS];

  DiscontState   discont_state;

  /* the timestamp of the next frame */
  GstClockTime   next_time;
  gint64         segment_start;
  gint64         segment_end;

  /* video state */
  Mpeg2decFormat format;
  gint           width;
  gint           height;
  gint           decoded_width;
  gint           decoded_height;
  gint           pixel_width;
  gint           pixel_height;
  gint           frame_rate_code;
  gint64         total_frames;
  gint64         frame_period;
  
  guint64        offset;
  gint           fps_n;
  gint           fps_d;
  gboolean       need_sequence;

  GstEvent      *pending_event;

  GstIndex      *index;
  gint           index_id;
  
  gint           error_count;
};

struct _GstMpeg2decClass {
  GstElementClass parent_class;
};

GType gst_mpeg2dec_get_type(void);

G_END_DECLS

#endif /* __GST_MPEG2DEC_H__ */
