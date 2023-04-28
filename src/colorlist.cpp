#include"colorlist.h"

cpprich_colortype_t color_type = COLOR_8BIT;
// by default 8 bit color

const cpprich_displaysetlist_t Display_Set_List[] = {
// ========== text basic style ==========
    { "bold"             , "\033[1m"   },
    { "dim"              , "\033[2m"   },
    { "italic"           , "\033[3m"   },
    { "underline"        , "\033[4m"   },
    { "blick"            , "\033[5m"   },
// ========== foreground color ==========
    { "black"            , "\033[30m"  },
    { "red"              , "\033[31m"  },
    { "green"            , "\033[32m"  },
    { "yellow"           , "\033[33m"  },
    { "blue"             , "\033[34m"  },
    { "magenta"          , "\033[35m"  },
    { "cyan"             , "\033[36m"  },
    { "white"            , "\033[37m"  },
    { "grey"             , "\033[90m"  },
    { "bright black"     , "\033[90m"  },
    { "bright red"       , "\033[91m"  },
    { "bright green"     , "\033[92m"  },
    { "bright yellow"    , "\033[93m"  },
    { "bright blue"      , "\033[94m"  },
    { "bright magenta"   , "\033[95m"  },
    { "bright cyan"      , "\033[96m"  },
    { "bright white"     , "\033[97m"  },
// ========== background color ==========
    { "b-black"          , "\033[40m"  },
    { "b-red"            , "\033[41m"  },
    { "b-green"          , "\033[42m"  },
    { "b-yellow"         , "\033[43m"  },
    { "b-blue"           , "\033[44m"  },
    { "b-magenta"        , "\033[45m"  },
    { "b-cyan"           , "\033[46m"  },
    { "b-white"          , "\033[47m"  },
    { "b-grey"           , "\033[100m" },
    { "b-bright black"   , "\033[100m" },
    { "b-bright red"     , "\033[101m" },
    { "b-bright green"   , "\033[102m" },
    { "b-bright yellow"  , "\033[103m" },
    { "b-bright blue"    , "\033[104m" },
    { "b-bright magenta" , "\033[105m" },
    { "b-bright cyan"    , "\033[106m" },
    { "b-bright white"   , "\033[107m" }
};