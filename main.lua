-- Load some default values for our rectangle.
function love.load(arg)
    if arg[#arg] == "-debug" then require("mobdebug").start() end
end
 
user_x = 100
user_speed = 500 -- pixels per second

move_left = false
move_right = false

-- Increase the size of the rectangle every frame.
function love.update(dt)
  
  move_left = love.keyboard.isDown("left") or love.keyboard.isDown("a")
  move_right = love.keyboard.isDown("right") or love.keyboard.isDown("d")
  
  if (move_left and not move_right) then
    user_x = user_x - user_speed*dt
  elseif (move_right and not move_left) then
    user_x = user_x + user_speed*dt
  end
  
  move_left = false
  move_right = false
end
 

-- x,y is midpoint on rectangle on line
function calc_user_rect_points(x, y, flip)
  rect_size = 40
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
 

-- Draw a coloured rectangle.
function love.draw()
    w = love.graphics.getWidth()
    h = love.graphics.getHeight()
    
    draw_user_rect()

end