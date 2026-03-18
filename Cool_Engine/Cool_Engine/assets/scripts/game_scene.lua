local TX   = game.TABLE_X;  local TY   = game.TABLE_Y
local TW   = game.TABLE_W;  local TH   = game.TABLE_H
local WT   = game.WALL_T;   local BR   = game.BALL_R
local PR   = game.POCKET_R; local MD   = game.MAX_DRAG
local MI   = game.MAX_IMPULSE

local S_AIMING  = "AIMING"
local S_MOVING  = "BALLS_MOVING"
local S_IN_HAND = "BALL_IN_HAND"
local S_OVER    = "GAME_OVER"

local FELT    = rl.make_color(34, 110, 55, 255)
local CUSHION = rl.make_color(22, 100, 28, 255)
local WOOD    = rl.make_color(90,  48, 12, 255)

local score         = 0
local dragging      = false
local drag_x, drag_y = 0, 0

local function vlen(x, y)   return math.sqrt(x*x + y*y) end
local function vnorm(x, y)
    local l = vlen(x, y)
    if l < 1e-6 then return 0, 0 end
    return x/l, y/l
end
local function vdist(x1,y1,x2,y2)
    return vlen(x2-x1, y2-y1)
end

function update()
    local st = game.get_state()

    if st == S_MOVING then
        local n, cue_fell = game.check_pockets()
        score = score + n

        if game.ball_count() == 0   then game.set_state(S_OVER);    return end
        if cue_fell                 then game.set_state(S_IN_HAND);  return end
        if game.all_stopped()       then game.set_state(S_AIMING)          end
        return
    end

    if st == S_IN_HAND then
        local mx, my = rl.mouse_x(), rl.mouse_y()
        local ok = mx > TX + BR and mx < TX + TW - BR
               and my > TY + BR and my < TY + TH - BR

        if rl.mouse_pressed(rl.MOUSE_LEFT) and ok then
            game.place_cue_ball(mx, my)
            game.set_state(S_AIMING)
        end
        return
    end

    if st == S_AIMING then
        local cbx, cby, has_cue = game.cue_ball_pos()
        if not has_cue then return end

        local mx, my = rl.mouse_x(), rl.mouse_y()

        if rl.mouse_pressed(rl.MOUSE_LEFT) then
            if vdist(mx, my, cbx, cby) < BR * 4.0 then
                dragging = true
                drag_x, drag_y = mx, my
            end
        end

        if dragging and rl.mouse_down(rl.MOUSE_LEFT) then
            drag_x, drag_y = mx, my
        end

        if dragging and rl.mouse_released(rl.MOUSE_LEFT) then
            local dx, dy = cbx - drag_x, cby - drag_y
            local dist   = vlen(dx, dy)
            if dist > 5.0 then
                local power    = math.min(dist / MD, 1.0)
                local nx, ny   = vnorm(dx, dy)
                game.apply_shot(nx * power * MI, ny * power * MI)
                game.set_state(S_MOVING)
            end
            dragging = false
        end

        if rl.mouse_pressed(rl.MOUSE_RIGHT) then
            dragging = false
        end
    end
end


local function draw_table()
    rl.draw_rectangle(TX - WT - 6,  TY - WT - 6, TW + (WT+6)*2, TH + (WT+6)*2, WOOD)
    rl.draw_rectangle(TX - WT, TY - WT, TW + WT*2, TH + WT*2, CUSHION)
    rl.draw_rectangle(TX, TY, TW, TH, FELT)
    local lx = TX + TW * 0.25
    rl.draw_line(lx, TY, lx, TY + TH, rl.fade(rl.WHITE, 0.15))
    rl.draw_circle(TX + TW*0.65, TY + TH*0.5, 3, rl.fade(rl.WHITE, 0.3))
end

local function draw_pockets()
    for _, p in ipairs(game.get_pockets()) do
        rl.draw_circle(p.x, p.y, PR,       rl.BLACK)
        rl.draw_circle_lines(p.x, p.y, PR, rl.fade(rl.DARKGRAY, 0.6))
    end
end

local function draw_balls()
    for _, b in ipairs(game.get_balls()) do
        rl.draw_circle(b.x, b.y, BR, b.color)
        rl.draw_circle(b.x - BR*0.3, b.y - BR*0.35,
                       BR*0.28, rl.fade(rl.WHITE, 0.38))
    end

    local cbx, cby, ok = game.cue_ball_pos()
    if ok then
        rl.draw_circle(cbx, cby, BR, rl.WHITE)
        rl.draw_circle(cbx - BR*0.3, cby - BR*0.35,
                       BR*0.32, rl.fade(rl.WHITE, 0.55))
    end
end

local function draw_aiming_ui()
    local st = game.get_state()

    if st == S_IN_HAND then
        local mx, my = rl.mouse_x(), rl.mouse_y()
        rl.draw_circle_lines(mx, my, BR, rl.fade(rl.WHITE, 0.65))
        rl.draw_text("Place cue ball  (left click)",
            TX + 10, TY + TH + 30, 18, rl.YELLOW)
        return
    end

    if st ~= S_AIMING then return end

    local cbx, cby, ok = game.cue_ball_pos()
    if not ok or not dragging then return end

    local dx, dy   = cbx - drag_x, cby - drag_y
    local dist     = vlen(dx, dy)
    local power    = math.min(dist / MD, 1.0)
    local nx, ny   = vnorm(dx, dy)

    local sx, sy   = drag_x - nx*20,    drag_y - ny*20
    local tx, ty   = cbx   - nx*(BR+3), cby   - ny*(BR+3)
    rl.draw_line_ex(sx, sy, tx, ty, 5.0, rl.BROWN)
    rl.draw_line_ex(sx, sy, tx, ty, 2.0, rl.fade(rl.BEIGE, 0.6))

    local tlen    = 80.0 + power * 120.0
    local ex, ey  = cbx + nx*tlen, cby + ny*tlen
    rl.draw_line_ex(cbx, cby, ex, ey, 1.5, rl.fade(rl.WHITE, 0.45))

    local ang  = math.atan(ny, nx)
    local alen = 10.0
    rl.draw_triangle(
        ex, ey,
        ex + math.cos(ang + 2.5)*alen,  ey + math.sin(ang + 2.5)*alen,
        ex + math.cos(ang - 2.5)*alen,  ey + math.sin(ang - 2.5)*alen,
        rl.fade(rl.WHITE, 0.45))

    local bx, by, bw, bh = 10, 10, 180, 18
    rl.draw_rectangle(bx-1, by-1, bw+2, bh+2, rl.DARKGRAY)
    rl.draw_rectangle(bx, by, bw, bh, rl.make_color(40,40,40,220))
    local bar_col = (power < 0.40 and rl.GREEN)
                 or (power < 0.75 and rl.YELLOW)
                 or rl.RED
    rl.draw_rectangle(bx, by, math.floor(bw*power), bh, bar_col)
    rl.draw_text("POWER", bx + bw + 8, by + 1, 16, rl.WHITE)
end

local function draw_hud()
    local st = game.get_state()
    local sw = rl.screen_width()
    local sh = rl.screen_height()

    rl.draw_text(string.format("Pocketed: %d / 15", score),
        10, 40, 20, rl.GOLD)

    if st == S_AIMING then
        local hint = dragging
            and "Release to shoot  |  Right-click to cancel"
            or  "Click near the cue ball and drag to aim"
        rl.draw_text(hint, 10, sh - 28, 16, rl.LIGHTGRAY)

    elseif st == S_MOVING then
        rl.draw_text("...", sw // 2 - 10, sh - 28, 18, rl.GRAY)

    elseif st == S_OVER then
        local msg = "YOU WIN!  All balls pocketed"
        local tw  = rl.measure_text(msg, 32)
        rl.draw_rectangle(sw//2 - tw//2 - 20, sh//2 - 30,
                          tw + 40, 60, rl.fade(rl.BLACK, 0.75))
        rl.draw_text(msg, sw//2 - tw//2, sh//2 - 16, 32, rl.GOLD)
    end
end

function draw()
    draw_table()
    draw_pockets()
    draw_balls()
    draw_aiming_ui()
    draw_hud()
end

rl.print("game_scene.lua loaded OK")