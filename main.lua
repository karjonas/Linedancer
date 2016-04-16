opponents = {}
 
user_x = 300
user_speed = 500 -- pixels per second

move_left = false
move_right = false
  
rect_size = 40

is_debug = false

function rect_opponent_right(x)
  return {speed = 100, x = x, direction = "right"}  
end

function rect_opponent_left(x)
  return {speed = 100, x = x, direction = "left"}  
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
    table.insert(opponents, rect_opponent_left(400))
    table.insert(opponents, rect_opponent_right(-100))

end

-- Increase the size of the rectangle every frame.
function collide()
  num_pts = table.getn(opponents)
  
  i = 1
  while (i <= num_pts) do
    if overlaps(user_x, opponents[i].x, rect_size) then
      table.remove(opponents, i)
      i = i - 1
      num_pts = num_pts - 1
    end
    i = i + 1
  end  

end
 

function love.update(dt)
  
  dt = is_debug and math.min(dt,0.1) or dt
  
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
  
  collide()
  
end
 

-- x,y is midpoint on rectangle on line
function calc_user_rect_points(x, y, flip)
  rect_half = rect_size/2

  sign = flip and -1 or 1

  p0 = {x = x - rect_half, y = y                  }
  p1 = {x = x - rect_half, y = y + sign*rect_size }
  p2 = {x = x + rect_half, y = y + sign*rect_size }
  p3 = {x = x + rect_half, y = y                  }
  
  return {p0,p1,p2,p3}
end

function draw_user_rect()
  w = love.graphics.getWidth()
  h = love.graphics.getHeight()
 
  points = calc_user_rect_points(user_x, h/2, false)
  
  table.insert(points, 1, {x = 0, y = h/2}) -- start_pt
  table.insert(points, table.getn(points) + 1, {x = w, y = h/2}) -- end_pt
 
  love.graphics.setColor(0, 100, 100)
  
  num_pts = table.getn(points)
  
  for i = 1, num_pts - 1 do
    fst, snd = points[i], points[i+1]
    love.graphics.line(fst.x, fst.y, snd.x, snd.y)
  end  
end

function draw_opponent_rect(opponent)
  w = love.graphics.getWidth()
  h = love.graphics.getHeight()
 
  points = calc_user_rect_points(opponent.x, h/2, false)
 
  love.graphics.setColor(100, 100, 0)
  
  num_pts = table.getn(points)
  
  for i = 1, num_pts - 1 do
    fst, snd = points[i], points[i+1]
    love.graphics.line(fst.x, fst.y, snd.x, snd.y)
  end  
end
 

-- Draw a coloured rectangle.
function love.draw()
    w = love.graphics.getWidth()
    h = love.graphics.getHeight()
    
    draw_user_rect()
    
    for key, value in pairs(opponents) do
      draw_opponent_rect(value)
    end

end