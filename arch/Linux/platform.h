/* This file contains platform settings for Linux */

/* at least my Linux box doesn't have ascftime, so we've got to fall back to asctime() */
#undef	HAVE_ASCFTIME
