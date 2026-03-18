local circles = {}
print("LOAD!!!!")


function update()
	print("UPDATE!!")
		if rl.mouse_pressed(1) then
			local sx = rl.mouse_x()
			local sy = rl.mouse_y()
			print("Mouse clicked")
			--local wx, wy = rl.to_world(sx, sy)
			--print("Mundo: " .. wx .. "," .. wy)
			table.insert(circles, {x = sx, y = sy, radius = 10.0})
	end
end

function draw() 

	for _, circle in ipairs(circles) do
		rl.draw_circle(circle.x, circle.y, circle.radius)
	end

end