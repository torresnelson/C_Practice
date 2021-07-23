// ejemplos para avl

struct rep_avl {
	info_t dato;
	nat altura;
	nat cantidad;
	avl_t izq, der;
};


static info_t *en_orden_rec(info_t *res, nat &tope, avl_t avl) {
	if (!es_vacio_avl(avl)) {
		res = en_orden_rec(res, tope, izq_avl(avl));
		res[tope] = raiz_avl(avl);
		tope++;
		res = en_orden_rec(res, tope, der_avl(avl));
	}
	return res;
}



static avl_t a2avl_rec(info_t *infos, int inf, int sup) {
	avl_t res;
	if (inf > sup)
		res = NULL;
	else {
		nat medio = (inf + sup) / 2;
		res = new rep_avl;
		res->dato = infos[medio];
		res->izq = a2avl_rec(infos, inf, medio -1);
		res->der = a2avl_rec(infos, medio + 1, sup);
		// ajustar res->altura y res->cantidad
	}
	return res;
}

info_t *en_orden_avl(avl_t avl) {
	info_t *res;
	if (es_vacio_avl(avl))
		res = NULL;
	else {
		res = new info_t[cantidad_en_avl(avl)];
		nat tope = 0;
		res = en_orden_rec(res, tope, avl);
	}
	return res;
}

avl_t arreglo_a_avl(info_t *infos, nat n) {
	return a2avl_rec(infos, 0, n - 1);
}

struct avl_ultimo {
	avl_t avl;
	int ultimo;
};
static avl_ultimo avl_min_rec(nat h, nat primero) {
	avl_ultimo res;
	if (h == 0) {
		res.avl = NULL;
		res.ultimo = primero -1;
	} else if (h == 1) {
		// Completar
	} else {
		//Completarnto
	}
	return res;
}
avl_t avl_min(nat h) {
	avl_ultimo res= avl_min_rec(h, 1);
	return res.avl;
}

//Ejemplo para conjuntos

struct res_conjuntos {
	avl_t arbol;
};

iterador_t iterador_conjunto(conjunto_t  c) {
	iterador_t res = crear_iterador();
	info_t *infos = en_orden_avl(c->arbol);
	for (nat i = 0; i < cantidad_en_avl(c->arbol); i++)
		agregar_a_iterador(infos[i], res);
	bloquear_iterador(res);
	delete[] infos;
	return res;
}
