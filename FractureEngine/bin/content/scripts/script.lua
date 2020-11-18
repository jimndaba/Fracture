
Require "FractureCore.lua"

local rotationSpeed;

function start()
end

function update (dt)

  Rotate(rotationSpeed * dt)
  print(dt)


end

function onCollision(collision c)
  -- body...
end
