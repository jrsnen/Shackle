

function loadContext(context)
    context:luaAddActionMap(0x01,"QUIT") -- K1 to QUIT
    context:luaAddStateMap(0x02,"SCROLLUP")  -- K2 to scrollup
    context:luaAddRangeMap(0x00,"MOUSEX")  -- MOUSEXREL to MOUSEX
end

