/* See LICENSE file for copyright and license details. */
#include <string.h>
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = {"Liberation Mono:size=11", "Font Awesome 5 Free:size=11" };
static const char dmenufont[]       = "Font Awesome 5 Free:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "dc", "KP", "m", "g", "rss", "t", "w" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{"KeePassXC",NULL,     NULL,           1 << 8,    0,          0,           0,        -1},
	{NULL,       "noSwallow",NULL,	       0,	  0,	      0,	   1,	     -1},
	{NULL,	     NULL,     "newsboat",     1 << 11,	  0,	      0,	   0,	     -1},
	{NULL,       NULL,     "vimpc",        1 << 9,    0,          0,           0,        -1},
	{NULL,       NULL,    "Discord Updater",1 << 7,   0,          0,           0,        -1},
	{NULL,       NULL,    "Discord",       1 << 7,    0,          0,           0,        -1},
	{"MultiMC5", NULL,     NULL,	       1 << 10,   0,	      0,	   0,	     -1},
	{"Minecraft", "Minecraft", NULL,       1 << 10,   0,	      0,	   0,	     -1},
	{NULL,	     NULL,     "Steam",	       1 << 10,   1,	      0,	   0,	     -1},
	{"Steam",    NULL,     NULL,	       1 << 10,   1,	      0,	   0,	     -1},
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1},
	{"Microsoft Teams - Preview", NULL, NULL, 1 << 12, 0,	      0,	   0,	      -1},
	{"Rambox",   NULL,     NULL,	       1 << 13,	  0,	      0,	   0,	      -1},
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static char dmenucmdstr[256] = "--dmenu=\"";
static const char *j4_dmenu_desktopcmd[] = {"j4-dmenu-desktop", dmenucmdstr, NULL};
static const char *termcmd[]  = { "st", "-f", "Liberation Mono:size=10.5", NULL };
static const char *firefoxcmd[] = {"firefox", NULL};
static const char *sleepcmd[] = {"systemctl", "suspend", NULL};
static const char *shutdowncmd[] = {"systemctl", "poweroff", NULL};
static const char *lockcmd[] = {"systemctl", "lock-session", NULL};
static const char *rebootcmd[] = {"systemctl", "reboot", NULL};
static const char *screenshotcmd[] = {"screenshot", NULL};
static const char *mpcTogglecmd[] = {"mpc", "toggle", NULL};
static const char *mpcNextcmd[] = {"mpc", "next", NULL};
static const char *mpcPrevcmd[] = {"mpc", "prev", NULL};
static const char *mpcBegincmd[] = {"mpc", "seek", "0", NULL};
static const char *droidcamTogglecmd[] = {"droidcamToggle", NULL};
static const char *dummycmd[] = {"echo", "-n", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = j4_dmenu_desktopcmd} },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_o,      spawn,          {.v = firefoxcmd} },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = sleepcmd}},
	{ MODKEY|ShiftMask,             XK_h,      spawn,          {.v = shutdowncmd}},
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lockcmd}},
	{ MODKEY|ShiftMask,             XK_r,      spawn,          {.v = rebootcmd}},
	{ MODKEY,			XK_s,	   spawn,	   {.v = screenshotcmd}},
	{ 0,				XF86XK_AudioPlay,spawn,	   {.v = mpcTogglecmd}},
	{ 0,				XF86XK_AudioNext,spawn,	   {.v = mpcNextcmd}},
	{ 0,				XF86XK_AudioPrev,spawn,	   {.v = mpcPrevcmd}},
	{ 0,				XF86XK_AudioStop,spawn,	   {.v = mpcBegincmd}},
	{ MODKEY,			XK_x,	   spawn,	   {.v = droidcamTogglecmd}},
	{ MODKEY,			XK_odiaeresis, spawn,	   {.v = dummycmd}},
	{ MODKEY,			XK_m,	   spawn, 	   {.v = dummycmd}},
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	//{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	//{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	//{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_e,                      7)
	TAGKEYS(                        XK_w,                      8)
	TAGKEYS(			XK_y,			   9)
	TAGKEYS(			XK_g,			   10)
	TAGKEYS(			XK_u,			   11)
	TAGKEYS(			XK_z,			   12)
	TAGKEYS(			XK_a,			   13)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
#include <stdio.h>
void config_start() {
	for (unsigned i = 0; dmenucmd[i] != NULL; ++i) {
		strcat(dmenucmdstr, dmenucmd[i]);
		strcat(dmenucmdstr, "\" \"");
	}
	dmenucmdstr[strlen(dmenucmdstr) - 2] = '\0';
	//printf("%s\n", dmenucmdstr);
}
