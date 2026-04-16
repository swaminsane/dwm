/* See LICENSE file for copyright and license details. */
// test
#include "/home/swaminsane/.config/theme/colors.h"
#include "/home/swaminsane/.config/theme/fonts.h"
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx = BORDER_PX; /* border pixel of windows */
static const unsigned int snap = 20;            /* snap pixel */
static const int swallowfloating =
    0;                        /* 1 means swallow floating windows by default */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1;  /* 0 means bottom bar */
static const char *fonts[] = {
    FONT_MAIN, "Noto Color Emoji:pixelsize=11:antialias=true:autohint=true"};
static const char dmenufont[] = FONT_MAIN;

static const char *colors[][3] = {
    [SchemeNorm] = {COL_FG, COL_BG, COL_BORDER},
    [SchemeSel] = {COL_BG, COL_ACCENT, COL_ACCENT},
};
/* tagging */
static const char *tags[] = {"α", "β", "γ", "δ", "ε", "ζ"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp", NULL, NULL, 0, 1, -1},
    //        { "St",       NULL,       NULL,       0,            0, -1 },
    {"St", NULL, "notes", 1 << 2, 0, -1},
    {NULL, NULL, NULL, 0, 0, -1},
    {"firefox", NULL, NULL, 0, 0, -1},
    //        { "fzf-pop",  NULL,       NULL,       0,            1, -1 },
};

// helper macros
#define TAGMASK ((1 << LENGTH(tags)) - 1)

void viewadjacent(const Arg *arg) {
  int seltag = 0;
  for (int i = 0; i < LENGTH(tags); i++) {
    if (selmon->tagset[selmon->seltags] & (1 << i)) {
      seltag = i;
      break;
    }
  }

  int nexttag = (seltag + arg->i + LENGTH(tags)) % LENGTH(tags);
  Arg a = {.ui = 1 << nexttag};
  view(&a);
}

void tagadjacent(const Arg *arg) {
  if (!selmon->sel)
    return;

  int seltag = 0;
  for (int i = 0; i < LENGTH(tags); i++) {
    if (selmon->sel->tags & (1 << i)) {
      seltag = i;
      break;
    }
  }

  int nexttag = (seltag + arg->i + LENGTH(tags)) % LENGTH(tags);
  Arg a = {.ui = 1 << nexttag};
  tag(&a);
  view(&a);
}

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */
static const int refreshrate =
    120; /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m",      dmenumon,
                                 "-fn",       dmenufont, NULL};
// static const char *termcmd[]    = { "st", NULL };
static const char *tabtermcmd[] = {"tabbed", "-r 2", "st", "-w", "''", NULL};
static const char *notescmd[] = {
    "st", "-t", "notes", "e", "nvim", "$HOME/sync/docs/notes/quicknotes.md",
    NULL};

// static const char *surfcmd[] = { "surf", "https://startpage.com", NULL };

/*
static const char *surfcmd[] = {
    "sh", "-c",
    "wid=$(tabbed -d); "
    "surf -e $wid http://localhost:8080 & ",
    NULL
};
*/

// static const char *fzfcmd[]      = { "st", "-c", "fzf-pop", "-g",
// "1886x1046+17+17", "-e", "fzfsearch", NULL };
static const char *fzfcmd[] = {"$HOME/.local/bin/fzfsearch", NULL};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    //    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    //	{ MODKEY,                       XK_Return, spawn,          {.v =
    // termcmd} },
    {MODKEY, XK_Return, spawn, {.v = tabtermcmd}},
    {MODKEY | ShiftMask, XK_Return, spawn,
     SHCMD("$HOME/.local/bin/st-samedir")},
    {MODKEY, XK_f, spawn, SHCMD("$HOME/.local/bin/startpager")},
    {MODKEY | ShiftMask, XK_f, spawn, SHCMD("firefox")},
    {MODKEY, XK_Home, spawn, SHCMD("pcmanfm")},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_Down, focusstack, {.i = +1}},
    {MODKEY, XK_Up, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_Left, setmfact, {.f = -0.05}},
    {MODKEY, XK_Right, setmfact, {.f = +0.05}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_c, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    //	{ MODKEY,                       XK_f,      spawn,          {.v =
    // librewolf } },
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5){MODKEY | ShiftMask, XK_q, quit, {0}},
    {MODKEY, XK_f, spawn, {.v = fzfcmd}},
    {MODKEY | ShiftMask, XK_l, spawn, {.v = notescmd}},
    //    {MODKEY, XK_t, spawn, SHCMD("st -e $HOME/.local/bin/hub")},
    {MODKEY, XK_e, spawn, SHCMD("st -e nvim")},

    // My additions
    {0,
     XK_F4,
     spawn,
     {.v = (const char *[]){"amixer", "set", "Capture", "toggle", NULL}}},
    {MODKEY, XK_n, spawn,
     SHCMD("st -t notes -e nvim $HOME/sync/docs/notes/quicknotes.md")},
    // Function
    {0, XF86XK_AudioMute, spawn, SHCMD("amixer set Master toggle")},
    {0, XF86XK_AudioRaiseVolume, spawn, SHCMD("amixer set Master 1%+")},
    {0, XF86XK_AudioLowerVolume, spawn, SHCMD("amixer set Master 1%-")},
    //    { 0, XF86XK_AudioMicMute, spawn, SHCMD("amixer set Capture toggle") },
    {0, XF86XK_MonBrightnessUp, spawn, SHCMD("brightnessctl set +1%")},
    {0, XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl set 1%-")},
    // D menooo
    {Mod1Mask, XK_F12, spawn,
     SHCMD("$HOME/.local/bin/display-init && $HOME/.local/bin/toggle-display")},
    {Mod1Mask, XK_space, spawn, {.v = dmenucmd}},
    {Mod1Mask, XK_b, spawn, SHCMD("$HOME/.local/bin/menu/surfmenu")},
    {Mod1Mask, XK_f, spawn, SHCMD("st -e lf")},
    {Mod1Mask, XK_m, spawn, SHCMD("$HOME/.local/bin/menu/music/musicmenu")},
    {Mod1Mask, XK_p, spawn, SHCMD("$HOME/.local/bin/menu/pomodmenu")},
    {0, XK_Print, spawn, SHCMD("$HOME/.local/bin/menu/scrshotmenu")},
    {Mod1Mask, XK_Escape, spawn, SHCMD("$HOME/.local/bin/menu/powermenu")},
    {Mod1Mask, XK_F10, spawn, SHCMD("$HOME/.local/bin/menu/connectmenu")},
    {Mod1Mask, XK_Super_L, spawn, SHCMD("$HOME/.local/bin/menu/allmenu")},
    /*
        {Mod1Mask, XK_y, spawn, SHCMD("$HOME/.local/bin/menu/ytdmenu")},
        {Mod1Mask, XK_v, spawn, SHCMD("$HOME/.local/bin/menu/clipmenu")},
        {Mod1Mask, XK_y, spawn, SHCMD("$HOME/.local/bin/menu/ytdmenu")},
        {Mod1Mask, XK_r, spawn, SHCMD("$HOME/.local/bin/menu/rssmenu")},
        {Mod1Mask, XK_t, spawn, SHCMD("$HOME/.local/bin/menu/textsmenu")},
        {Mod1Mask, XK_Insert, spawn, SHCMD("$HOME/.local/bin/menu/passmenu")},
        {Mod1Mask | ShiftMask, XK_t, spawn,
       SHCMD("$HOME/.local/bin/menu/themenu")}, {Mod1Mask, XK_F7, spawn,
       SHCMD("$HOME/.local/bin/menu/tvmenu")},
    */
    // View next/prev tag
    {MODKEY, XK_h, viewadjacent, {.i = -1}},
    {MODKEY, XK_l, viewadjacent, {.i = +1}},

    // Move window to next/prev tag
    {MODKEY, XK_j, tagadjacent, {.i = -1}},
    {MODKEY, XK_k, tagadjacent, {.i = +1}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = tabtermcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
