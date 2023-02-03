/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 15;       /* gap pixel between windows */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;   /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 5;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const unsigned int snap      = 0;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 1;        /* vertical padding of bar */
static const int sidepad            = 1;        /* horizontal padding of bar */
static const char *fonts[]          = { "Fira Code Nerd Font Complete:size=12", "Symbola:size=12" };
static const char dmenufont[]       = "Fira Code Nerd Font Complete:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005357";
/* BEGIN PYWAL COMMENT */
static const char *colors[][3]      = {
	//                      fg          bg           border   //
	[SchemeNorm]        = { col_gray3, col_gray1,   col_gray2  },
	[SchemeSel]         = { col_gray4, col_cyan,    col_cyan   },
	[SchemeStatus]      = { col_gray3, col_cyan,    "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]     = { col_gray4, col_cyan,    "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]    = { col_gray3, col_gray1,   "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]     = { col_gray4, col_cyan,    "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]    = { col_gray3, col_gray1,   "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};
/* END PYWAL COMMENT */ 
// #include "/home/began/.cache/wal/colors-wal-dwm.h"

static const char *const autostart[] = {
        "/usr/lib/notification-daemon-1.0/notification-daemon", "&", NULL,
        "rsblocks","&", NULL,
        "/usr/lib/polkit-kde-authentication-agent-1","&", NULL,
        "feh",
            "--bg-scale",
            "--no-fehbg",
            "/home/began/.local/share/autowall/paper", 
            NULL,
        "picom", "-b",
            NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const char *tagsel[][2] = {
	{ "#ffffff", "#005357" },
	{ "#ffffff", "#005357" },
	{ "#ffffff", "#005357" },
	{ "#ffffff", "#005357" },
	{ "#ffffff", "#005357" },
	{ "#ffffff", "#005357" },
	{ "#ffffff", "#005357" },
	{ "#000000", "#f0aa00" },
	{ "#ffffff", "#008800" },
};

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
	{ "[T]",      tile },    /* first entry is default */
	{ "<F>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|U|",      centeredmaster },
	{ ">O>",      centeredfloatingmaster },
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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-F", NULL };
static const char *dmenuwinecmd[] = { "/home/began/.local/bin/dmenuwine", NULL };
// static const char *dmenucmd[]    = { "rofi","-show", "drun", NULL };
static const char *termcmd[]     = { "st", NULL };
static const char *screenshot[]  = { "flameshot", "gui", NULL };
static const char *killdwm[]     = { "pkill", "dwm", NULL };
static const char *bupprim[]     = { "bright", "DP-4", "inc", NULL};
static const char *bdownprim[]   = { "bright", "DP-4", "dec", NULL};
static const char *bupsec[]      = { "bright", "HDMI-0", "inc", NULL};
static const char *bdownsec[]    = { "bright", "HDMI-0", "dec", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,           spawn,          {.v = dmenucmd} },
	{ MODKEY|ShiftMask,             XK_p,           spawn,          {.v = dmenuwinecmd} },
	{ MODKEY,                       XK_Return,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_Print,       spawn,          {.v = screenshot } },
	{ MODKEY,                       XK_minus,       setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,       setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,       setgaps,        {.i = 0  } },
	{ MODKEY,                       XK_b,           togglebar,      {0} },
	{ MODKEY,                       XK_j,           focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,           focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,           incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,           incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,           setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,           setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,      zoom,           {0} },
	{ MODKEY,                       XK_Tab,         view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,           killclient,     {0} },
	{ MODKEY,                       XK_t,           setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,           setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,           setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,       setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },
	{ MODKEY,                       XK_0,           view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,           tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_s,           focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_s,           tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F8,          spawn,         SHCMD("pamixer -i 10")},
	{ MODKEY,                       XK_F7,          spawn,         SHCMD("pamixer -d 10")},
	{ MODKEY|ShiftMask,             XK_F8,          spawn,         SHCMD("pamixer -i 2")},
	{ MODKEY|ShiftMask,             XK_F7,          spawn,         SHCMD("pamixer -d 2")},
	{ MODKEY,                       XK_F3,          spawn,          { .v = bdownprim} },
	{ MODKEY,                       XK_F4,          spawn,          { .v = bupprim} },
	{ MODKEY|ShiftMask,             XK_F3,          spawn,          { .v = bupsec} },
	{ MODKEY|ShiftMask,             XK_F4,          spawn,          { .v = bdownsec} },
	{ MODKEY|ControlMask|ShiftMask, XK_l,           spawn,         SHCMD("slock")},
	TAGKEYS(                        XK_1,                           0)
	TAGKEYS(                        XK_2,                           1)
	TAGKEYS(                        XK_3,                           2)
	TAGKEYS(                        XK_4,                           3)
	TAGKEYS(                        XK_5,                           4)
	TAGKEYS(                        XK_6,                           5)
	TAGKEYS(                        XK_7,                           6)
	TAGKEYS(                        XK_8,                           7)
	TAGKEYS(                        XK_9,                           8)
	{ MODKEY|ControlMask|ShiftMask, XK_e,           quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_Escape,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_Escape,      spawn,          {.v = killdwm }},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,      movemouse,      {.i = 1} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button3,      resizemouse,    {.i = 1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

