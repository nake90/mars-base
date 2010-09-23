-- Sistema de control de electricidad

elementos[]
objetos = 0

function create_new_entity(mode)
	objetos = objetos + 1
	if mode.tipo = "generador"
		elementos[objetos].tipo = "generador"
		elementos[objetos].consumo = - mode.generador
	else
		elementos[objetos].tipo = "normal"
		elementos[objetos].consumo = mode.consumo
	end
	return objetos
end

function conectar(id1, id2)
	
end