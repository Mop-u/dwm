/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";

static char normfgcolor[]     = "#bbbbbb";
static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char selfgcolor[]      = "#eeeeee";
static char selbgcolor[]      = "#005577";
static char selbordercolor[]  = "#005577";
static char black[]      = "#000000";
static char darkred[]    = "#5f0000";
static char darkgreen[]  = "#126600";
static char darkyellow[] = "#6d5700";
static char darkblue[]   = "#00196f";
static char darkmagenta[]= "#5e007d";
static char darkcyan[]   = "#00806a";
static char lightgrey[]  = "#cccccc";
static char darkgrey[]   = "#999999";
static char red[]        = "#eb1515";
static char green[]      = "#58d93c";
static char yellow[]     = "#ffd429";
static char blue[]       = "#3662f5";
static char magenta[]    = "#c670e3";
static char cyan[]       = "#71e8d4";
static char white[]      = "#ffffff";

static char *termcolor[] = {
  black,
  darkred,
  darkgreen,
  darkyellow,
  darkblue,
  darkmagenta,
  darkcyan,
  lightgrey,
  darkgrey,
  red,
  green,
  yellow,
  blue,
  magenta,
  cyan,
  white,
};

enum { SchemeNorm, SchemeCol1, SchemeCol2, SchemeCol3, SchemeCol4,
       SchemeCol5, SchemeCol6, SchemeSel }; /* color schemes */

static char *colors[][3]      = {
	/*               fg           bg          border   */
	[SchemeNorm]  = { normfgcolor,normbgcolor,normbordercolor },
	[SchemeCol1]  = { darkred,    normbgcolor,normbordercolor },
	[SchemeCol2]  = { darkgreen,  normbgcolor,normbordercolor },
	[SchemeCol3]  = { darkyellow, normbgcolor,normbordercolor },
	[SchemeCol4]  = { darkblue,   normbgcolor,normbordercolor },
	[SchemeCol5]  = { darkmagenta,normbgcolor,normbordercolor },
	[SchemeCol6]  = { darkcyan,   normbgcolor,normbordercolor },
	[SchemeSel]   = { selfgcolor, selbgcolor, selbordercolor },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
  { "TTT",      bstack },      /* first entry is default */
  { "===",      bstackhoriz },
	{ "[]=",      tile },        /* original tiling mode */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *volupcmd[] =      {"wpctl", "set-volume", "-l", "1.0", "@DEFAULT_AUDIO_SINK@", "5%+", NULL};
static const char *voldncmd[] =      {"wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "5%-", NULL};
static const char *togglemutecmd[] = {"wpctl", "set-mute", "@DEFAULT_AUDIO_SINK@", "toggle", NULL};
static const char *medianextcmd[] = {"playerctl", "next", NULL};
static const char *mediaplaypausecmd[] = {"playerctl", "play-pause", NULL};
static const char *mediaprevcmd[] = {"playerctl", "previous", NULL};
static const char *mediastopcmd[] = {"playerctl", "stop", NULL};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
  { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_r,      reorganizetags, {0} },
  { MODKEY|ShiftMask,             XK_r,      distributetags, {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
  { MODKEY,                       XK_u,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_o,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  {MODKEY,                        XK_F5,     xrdb,           {.v = NULL} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
  { 0,                XF86XK_AudioPlay,      spawn,          {.v = mediaplaypausecmd } },
  { 0,                XF86XK_AudioNext,      spawn,          {.v = medianextcmd } },
  { 0,                XF86XK_AudioPrev,      spawn,          {.v = mediaprevcmd } },
  { 0,                XF86XK_AudioStop,      spawn,          {.v = mediastopcmd } },
  { 0,                XF86XK_AudioRaiseVolume, spawn,        {.v = volupcmd } },
  { 0,                XF86XK_AudioLowerVolume, spawn,        {.v = voldncmd } },
  { 0,                XF86XK_AudioMute,      spawn,          {.v = togglemutecmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

