opponents = {}
shapeshifts = {}

user_x = 300
user_speed = 500 -- pixels per second

move_left = false
move_right = false

rect_size = 40

is_debug = false
elapsed_time = 0

user_shape = "rectangle"

color_bg = {r = 33, g = 33, b = 32}
color1 = {r = 240, g = 241, b = 238}
color2 = {r = 19, g = 82, b = 162}
color3 = {r = 255, g = 212, b = 100}
color0 = {r = 251, g = 105, b = 100}

function opponent(time, speed, direction, shape)
  return { speed = speed, time = time, direction = direction, shape = shape, active = false, x = 0 }
end

function rect_opponent_right(time)
  return opponent(time, 100, "right", "rectangle")  
end

function rect_opponent_left(time)
  return opponent(time, 100, "left", "rectangle")  
end

function triangle_opponent_left(time)
  return opponent(time, 100, "left", "triangle")  
end

function triangle_opponent_right(time)
  return opponent(time, 100, "right", "triangle")  
end

function overlaps(v0,v1,width)
  v0min = v0 - width/2
  v0max = v0 + width/2
  v1min = v1 - width/2
  v1max = v1 + width/2

  return (v0min <= v1max and v1min <= v0max) 
end

-- Load some default values for our rectangle.
function love.load(arg)
  if arg[#arg] == "-debug" then
    require("mobdebug").start()
    is_debug = true
  end
  
  user_x = love.graphics.getWidth() / 2

  table.insert(opponents,   rect_opponent_left(3))
  table.insert(opponents,   rect_opponent_right(3))
  table.insert(opponents,   rect_opponent_left(6))
  table.insert(opponents,   rect_opponent_right(6))
  table.insert(shapeshifts, triangle_opponent_left(8))
  table.insert(opponents,   triangle_opponent_right(8))
  table.insert(opponents,   triangle_opponent_left(10))
  table.insert(shapeshifts, rect_opponent_right(10))
  table.insert(opponents,   rect_opponent_left(12))

  love.graphics.setBackgroundColor(color_bg.r, color_bg.g, color_bg.b)
end

-- Increase the size of the rectangle every frame.
function collide()
  
  num_pts = table.getn(opponents)
  i = 1
  while (i <= num_pts) do
    opponent = opponents[i]
    if overlaps(user_x, opponent.x, rect_size) then
      table.remove(opponents, i)
      i = i - 1
      num_pts = num_pts - 1
    end
    i = i + 1
  end
  
  num_pts = table.getn(shapeshifts)
  i = 1
  while (i <= num_pts) do
    shape = shapeshifts[i]
    if overlaps(user_x, shape.x, rect_size) then
      table.remove(shapeshifts, i)
      i = i - 1
      num_pts = num_pts - 1
      user_shape = shape.shape
    end
    i = i + 1
  end
end
 
function love.keypressed( key, scancode, isrepeat )
  if not is_debug then return end
  
  if key == "space" then
    if user_shape == "rectangle" then
      user_shape = "triangle"
    else
      user_shape = "rectangle"
    end
  end
  
end

function love.update(dt)
  
  dt = is_debug and math.min(dt,0.1) or dt
  
  elapsed_time = elapsed_time + dt
  
  window_width = love.graphics.getWidth()
  
  for i=1, table.getn(shapeshifts) do
    shape = shapeshifts[i] 
    if (not shape.active) and (shape.time >= elapsed_time) then
      shape.active = true
      shape.x = (shape.direction == "left") and (window_width + 100) or (-100)
      shapeshifts[i] = shape
    end
  end
  
  for i=1, table.getn(opponents) do
    shape = opponents[i] 
    if (not shape.active) and (shape.time >= elapsed_time) then
      shape.active = true
      shape.x = (shape.direction == "left") and (window_width + 100) or (-100)
      opponents[i] = shape
    end
  end
  
  move_left = love.keyboard.isDown("left") or love.keyboard.isDown("a")
  move_right = love.keyboard.isDown("right") or love.keyboard.isDown("d")
  
  if (move_left and not move_right) then
    user_x = user_x - user_speed*dt
  elseif (move_right and not move_left) then
    user_x = user_x + user_speed*dt
  end
  
  user_x = math.max(0, user_x)
  user_x = math.min(love.graphics.getWidth(), user_x)
  
  move_left = false
  move_right = false
  
  for key, value in pairs(opponents) do
    sign = (value.direction == "left") and -1 or 1
    value.x = value.x + sign*value.speed*dt
  end
  
    for key, value in pairs(shapeshifts) do
    sign = (value.direction == "left") and -1 or 1
    value.x = value.x + sign*value.speed*dt
  end
  
  collide()
  
end
 

-- x,y is midpoint on rectangle on line
function calc_user_rectangle_points(x, y, flip)
  rect_half = rect_size/2

  sign = flip and 1 or -1

  p0 = {x = x - rect_half, y = y                  }
  p1 = {x = x - rect_half, y = y + sign*rect_size }
  p2 = {x = x + rect_half, y = y + sign*rect_size }
  p3 = {x = x + rect_half, y = y                  }
  
  return {p0,p1,p2,p3}
end

-- x,y is midpoint on triangle on line
function calc_user_triangle_points(x, y, flip)
  rect_half = rect_size/2

  sign = flip and 1 or -1

  p0 = {x = x - rect_half, y = y                  }
  p1 = {x = x            , y = y + sign*rect_half }
  p2 = {x = x + rect_half, y = y                  }
  
  return {p0,p1,p2}
end

function draw_user()
  w = love.graphics.getWidth()
  h = love.graphics.getHeight()
 
  points = {}
  if (user_shape == "triangle") then
    points = calc_user_triangle_points(user_x, h/2, false)
  else
    points = calc_user_rectangle_points(user_x, h/2, false)
  end

  table.insert(points, 1, {x = 0, y = h/2}) -- start_pt
  table.insert(points, table.getn(points) + 1, {x = w, y = h/2}) -- end_pt
 
  love.graphics.setColor(color0.r, color0.g, color0.b)
  
  num_pts = table.getn(points)
  
  for i = 1, num_pts - 1 do
    fst, snd = points[i], points[i+1]
    love.graphics.line(fst.x, fst.y, snd.x, snd.y)
  end  
end

function draw_opponent(opponent, color)
  w = love.graphics.getWidth()
  h = love.graphics.getHeight()

  points = {}
  if (opponent.shape == "triangle") then
    points = calc_user_triangle_points(opponent.x, h/2, false)
  else
    points = calc_user_rectangle_points(opponent.x, h/2, false)
  end
 
  love.graphics.setColor(color.r, color.g, color.b)
  
  num_pts = table.getn(points)
  
  for i = 1, num_pts - 1 do
    fst, snd = points[i], points[i+1]
    love.graphics.line(fst.x, fst.y, snd.x, snd.y)
  end
  
  love.graphics.setColor(color_bg.r, color_bg.g, color_bg.b)

  love.graphics.line(points[1].x, points[1].y, points[num_pts].x, points[num_pts].y)
end
 

-- Draw a coloured rectangle.
function love.draw()
    w = love.graphics.getWidth()
    h = love.graphics.getHeight()
    
    draw_user()
    
    for key, value in pairs(opponents) do
      if (value.active) then
        draw_opponent(value, color1)
      end
    end

    for key, value in pairs(shapeshifts) do
      if (value.active) then
        draw_opponent(value, color2)
      end
    end

end