-- Test

-- Modelos que se pueden usar POR IMPLEMENTAR
list_models = {
"hq_general_4_8x8",
"tapa2x2"
}

--[[
objeto = update(objeto, time_elapsed)
Esta funci�n se llama cada poco tiempo durante la ejecuci�n del programa.
Se pasan los datos del objeto que se pueden actualizar y despu�s se debe retornar.
A parte se pasa el tiempo en segundos que han pasado desde la �ltima vez que se llam� a esta funci�n
--]]

t=0

function update(objeto, time_elapsed)
	objeto.pos.x = objeto.pos.x + time_elapsed * math.sin(t);
	objeto.pos.y = objeto.pos.y + time_elapsed * math.cos(t);
	objeto.rot.yaw = - t / 0.01745329 -- Para que sea en grados ^^
	t=t+time_elapsed/5;
	if t >= 360 then
		t = t - 360
	end
	return objeto -- Siempre hay que retornar el objeto
end


--[[
objeto = create(objeto)
Se llama cada vez que se crea este objeto, para inicializar variables y cambiar datos.
--]]

function create(objeto)
	objeto.name = "Colono: nake"
	
	return objeto; -- Siempre hay que retornar el objeto
end

--[[
objeto = destroy(objeto)
Se llama cada vez que se borra este objeto, para desinicializar variables.
--]]

function destroy(objeto)
	-- No hacemos nada
	return objeto; -- Siempre hay que retornar el objeto
end
