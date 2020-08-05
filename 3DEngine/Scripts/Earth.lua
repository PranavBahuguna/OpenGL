ROTATION_SPEED = 60.0

function init()
  setPos(vec3.new(3.0, 0.0, 6.0))
  setScale(vec3.new(0.4))
end

function update()
  local angle = ROTATION_SPEED * getDeltaTime()
  rotate(vec3.new(0.0, angle, 0.0))
end