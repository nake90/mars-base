-- Generador de oxígeno

-- Modelos que se pueden usar POR IMPLEMENTAR
list_models = {
"hq_general_4_8x8",
"tapa2x2"
}

--[[
objeto = update(objeto, time_elapsed)
Esta función se llama cada poco tiempo durante la ejecución del programa.
Se pasan los datos del objeto que se pueden actualizar y después se debe retornar.
A parte se pasa el tiempo en segundos que han pasado desde la última vez que se llamó a esta función
--]]

function update(objeto, time_elapsed)
	objeto.node_data.gases.O2 = objeto.node_data.gases.O2 + 0.1 * time_elapsed
	
	return objeto -- Siempre hay que retornar el objeto
end


--[[
objeto = create(objeto)
Se llama cada vez que se crea este objeto, para inicializar variables y cambiar datos.
--]]

function create(objeto)
	objeto.name = "Generador de oxígeno"
	
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
