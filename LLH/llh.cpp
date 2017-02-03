#include "llh.h"

mensaje::mensaje()
{
	this->fecha = this->horaSat = this->lat = this->lon = this->alt = this->age = this->solStat = this->numSat = ""; 
	this->ratio = "0.0";
}

mensaje::~mensaje()
{
}
	
/*FUNCIONES DE TIPO GET*/
std::string mensaje::getFecha()
{
	return this->fecha;
}
	
std::string mensaje::getHoraSat()
{
	return this->horaSat;
}

std::string mensaje::getLat()
{
	return this->lat;
}

std::string mensaje::getLon()
{
	return this->lon;
}

std::string mensaje::getAlt()
{
	return this->alt;
}

std::string mensaje::getSolStat()
{
	return this->solStat;
}

std::string mensaje::getNumSat()
{
	return this->numSat;
}

stDev mensaje::getstDev()
{
	return this->staDev;
}

std::string mensaje::getAge()
{
	return this->age;
}

std::string mensaje::getRatio()
{
	return this->ratio;
}

/*FUNCIONES DE TIPO SET*/
void mensaje::setFecha(std::string f)
{
	std::cout << "Voy a asignar el string fecha: " << f << std::endl;
	this->fecha = f;
}
	
void mensaje::setHoraSat(std::string hs)
{
	std::cout << "Voy a asignar el string horasat: " << hs << std::endl;
	this->horaSat = hs;
}

void mensaje::setLat(std::string lt)
{
	std::cout << "Voy a asignar el string lat: " << lt << std::endl;
	this->lat = lt;
}

void mensaje::setLon(std::string ln)
{
	std::cout << "Voy a asignar el string lon: " << ln << std::endl;
	this->lon = ln;
}

void mensaje::setAlt(std::string at)
{
	std::cout << "Voy a asignar el string alt: " << at << std::endl;
	this->alt = at; 
}

void mensaje::setSolStat(std::string ss)
{
	std::cout << "Voy a asignar el string solstat: " << ss << std::endl;
	this->solStat = ss;
}

void mensaje::setNumSat(std::string ns)
{
	std::cout << "Voy a asignar el string numsat: " << ns << std::endl;
	this->numSat = ns;
}

int mensaje::setstDev(std::string * stdev) 
{
	return this->staDev.actualizaStDev(stdev);
}

void mensaje::setAge(std::string ag)
{
	std::cout << "Voy a asignar el string age: " << ag << std::endl;
	this->age = ag;
}

void mensaje::setRatio(std::string r)
{
	std::cout << "Voy a asignar el string ratio: " << r << std::endl;
	this->ratio = r;
}

int mensaje::actualizaMsj(std::string buffer)
{
	int value;
	int iteracion = 0;
	//value = buffer.find_first_of(" ");
	do
	{
		//Comentar todo el if si la solución del comentario de los switch-case es implementada.
		if((value = buffer.find_first_of(" ")) == buffer.npos)
		{
			//Hemos alcanzado la última parte del mensaje.
			//Esta parte debe corresponder al ratio.
			setRatio(buffer);
			return 0; //Asignación completa
		}

		switch(iteracion++)
		{
			//Posible solución: asignar valor value tras cada case.
			case 0:
				setFecha(buffer.substr(0, value));
				if(borraString(&buffer, value))
					return -1;
				break;
			case 1:
				setHoraSat(buffer.substr(0, value));
				if(borraString(&buffer, value))
					return -1;
				break;
			case 2:
				setLat(buffer.substr(0, value));
				if(borraString(&buffer, value))
					return -1;
				break;
			case 3:
				setLon(buffer.substr(0, value));
				if(borraString(&buffer, ++value))
					return -1;
				break;
			case 4:
				setAlt(buffer.substr(0, value));
				if(borraString(&buffer, value))
					return -1;
				break;
			case 5:
				setSolStat(buffer.substr(0, value));
				if(borraString(&buffer, value))
					return -1;
				break;
			case 6:
				setNumSat(buffer.substr(0, value));
				if(borraString(&buffer, value))
					return -1;
				break;
			case 7:
				if(setstDev(&buffer)) //Se hace el procesado de manera interna.
				{
					return -1;
				}
				break;
			case 8:
				setAge(buffer.substr(0, value));
				if(borraString(&buffer, value))
					return -1;
				break;
			case 9:
				setRatio(buffer.substr(0, value));
				if(borraString(&buffer, value))
					return -1;
				break;
			default:
				std::cout << std::endl;
				return 0;
		}
	}
	while(true);
}
