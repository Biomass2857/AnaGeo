#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

// Definition von Pi
const double pi = atan(1) * 4;

// ## Hilfsfunktionen ##
// unspezifisch und koennte genauso in jedem anderen Programm
// stehen, dieser Code trägt also nicht direkt zur
// Loesung des Problems bei
std::string trim(std::string str)
{
	const std::string remset = " \t\n\r";
	for(int i = 0; i < str.size(); i++)
	{
		for(int p = 0; p < remset.size(); p++)
		{
			if(str.at(i) == remset.at(p))
			{
				str.erase(str.begin() + i);
				i--;
			}
		}
	}
	return str;
}

std::vector<std::string> explode(std::vector<char> seps, std::string src, bool noticeEmpty = false)
{
	std::vector<std::string> ret;
	std::string tString = "";
	for(int i = 0; i < src.size(); i++)
	{
		bool found = false;
		for(int k = 0; k < seps.size(); k++)
		{
			if(src.at(i) == seps.at(k))
			{
				found = true;
				break;
			}
		}

		if(!found)
		{
			tString.push_back(src.at(i));
		}
		else
		{
			if(noticeEmpty || !tString.empty())
			{
				ret.push_back(tString);
				tString.clear();
			}
		}
	}

	if(noticeEmpty || !tString.empty())
		ret.push_back(tString);

	return ret;
}

std::vector<std::string> explode(char sep, std::string src, bool noticeEmpty = false)
{
	std::vector<std::string> ret;
	std::string tString = "";
	for(int i = 0; i < src.size(); i++)
	{
		if(src.at(i) != sep)
		{
			tString.push_back(src.at(i));
		}
		else
		{
			if(noticeEmpty || !tString.empty())
			{
				ret.push_back(tString);
				tString.clear();
			}
		}
	}

	if(noticeEmpty || !tString.empty())
		ret.push_back(tString);

	return ret;
}

bool in_array(std::string str, std::vector<std::string> array)
{
	for(int i = 0; i < array.size(); i++)
	{
		if(array.at(i) == str)
		{
			return true;
		}
	}

	return false;
}

double myAbs(double x) { return x < 0 ? -x : x; }

double fpff(double x, unsigned short st = 2) { return floor(pow(10, st) * x) / pow(10, st); }

// ## Hilfsfunktionen Ende ##

// Beschreibt den Typ einer linearen Gleichung
// Unterteilung in Informationsgrad selbiger
enum class Infotyp : unsigned short
{
	Keiner,
	Links,
	Rechts,
	Beide
};

// Beschreibt den Typ der Lage, beinhaltet alle moeglichen Lagebeziehungen im Raum
enum class Lage : unsigned short
{
	Identisch,
	Parallel,
	Schneiden,
	Windschief,
	Liegend
};

// Funktion zum Lösen eines Gleichungssystem mit den unbekannten r & s und 3 Gleichungen
// Ermittelung des Schnittpunkts
typedef struct
{
	double r;
	double l;
	bool loesbar;
}
Loesung;

// Die Klasse Vektor
// beinhaltet die Variablen x, y und z
// definiert sind einige Funktionen, die z.B. die Addition und Subtraktion
// von Vektoren erklären
// das gestaltet den nachfolgenden Programmierfluss weitaus angenehmer
class Vektor
{
	public:
		// Hilfsfunktionen
		Vektor() : x(0), y(0), z(0) {}
		Vektor(double tx, double ty, double tz) : x(tx), y(ty), z(tz) {}

		void operator=(Vektor other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}

		bool operator==(Vektor other)
		{
			return x == other.x && y == other.y && z == other.z;
		}

		bool operator!=(Vektor other)
		{
			return x != other.x || y != other.y || z != other.z;
		}

		Vektor operator-(Vektor other)
		{
			return Vektor(x - other.x, y - other.y, z - other.z);
		}

		Vektor operator+(Vektor other)
		{
			return Vektor(x + other.x, y + other.y, z + other.z);
		}

		// Funktion zur Multiplikation eines Vektor mit einem Skalar
		Vektor operator*(double sc)
		{
			return Vektor(sc * x, sc * y, sc * z);
		}

		Vektor operator/(double sc)
		{
			return Vektor(x / sc, y / sc, z / sc);
		}

		// Definition des Skalarprodukts
		double operator*(Vektor other)
		{
			return x * other.x + y * other.y + z * other.z;
		}

		// Definition des Kreuzprodukts
		// benutze das Prozentzeichen als x Äquivalent
		Vektor operator%(Vektor other)
		{
			return Vektor(y * other.z - other.y * z, z * other.x - other.z * x, x * other.y - other.x * y);
		}

		// Kreuzprodukt aufrollen, bzw NormalenVektor in zwei Vektoren umwandeln
		std::vector<Vektor> aufrollen()
		{
			std::vector<Vektor> ret(2);

			if(x != 0 && y != 0 && z != 0)
			{
				ret[0] = Vektor(y != 0 ? -y : 0, x, 0);
				ret[1] = Vektor(z != 0 ? -z : 0, 0, x);
			}
			else if(abs(x) != 0 && abs(y) != 0)
			{
				ret[0] = Vektor(y, -x, 0);
				ret[1] = Vektor(0, 0, 1);
			}
			else if(abs(y) != 0 && abs(z) != 0)
			{
				ret[0] = Vektor(0, -z, y);
				ret[1] = Vektor(1, 0, 0);
			}
			else if(abs(z) != 0 && abs(x) != 0)
			{
				ret[0] = Vektor(-z, 0, x);
				ret[1] = Vektor(0, 1, 0);
			}
			else if(abs(x) != 0)
			{
				ret[0] = Vektor(0, 1, 0);
				ret[1] = Vektor(0, 0, 1);
			}
			else if(abs(y) != 0)
			{
				ret[0] = Vektor(1, 0, 0);
				ret[1] = Vektor(0, 0, 1);
			}
			else if(abs(z) != 0)
			{
				ret[0] = Vektor(1, 0, 0);
				ret[1] = Vektor(0, 1, 0);
			}

			return ret;
		}

		// Betrag des Vektors
		double betrag()
		{
			return sqrt(x * x + y * y + z * z);
		}

		// Funktion zur Ermittelung ob 2 Vektoren parallel sind
		bool istKollinearZu(Vektor other)
		{
			double r = 0;
			if(other.x != 0)
			{
				r = x / other.x;
				return (y == r * other.y && z == r * other.z);
			}
			else if(x != 0)
				return false;
			else
				return true;
		}

		// Funktion zum Ermitteln des Schnittwinkels (Gradmass)
		double bekommeSchnittwinkelZuRichtungsvektor(Vektor other)
		{
			return 180.f * acos((*this * other) / (betrag() * other.betrag())) / pi;
		}

		double bekommeSchnittwinkelZuNormalenvektor(Vektor other)
		{
			return 180.f * asin((*this * other) / (betrag() * other.betrag())) / pi;
		}

		// Hilfsfunktion zum Einlesen eines Vektors
		bool parse(std::string s)
		{
			std::vector<std::string> parts;
			s = trim(s);
			parts = explode(',', s.substr(1, -1));
			if(parts.size() == 3)
			{
				x = std::stod(parts.at(0));
				y = std::stod(parts.at(1));
				z = std::stod(parts.at(2));
				return true;
			}
			else if(parts.size() == 2)
			{
				x = std::stod(parts.at(0));
				y = std::stod(parts.at(1));
				z = 0;
				return true;
			}
			else
				return false;
		}

		// erzeugt eine Zeichenkette, den Vektor
		// in der Form (x, y, z) darstellt
		std::string bekommePunktRepresentation()
		{
			std::stringstream ss;

			ss <<"("<< fpff(x) <<", "<< fpff(y) <<", "<< fpff(z) <<")";

			return ss.str();
		}

		double x;
		double y;
		double z;
};

// Hilfsfunktion für skalare Multiplikation
Vektor operator*(double sc, Vektor v)
{
	return v * sc;
}

// Die Klasse die eine lineare Gleichung der Form 
// m1 * x + n1 = m2 * y + n2
// repraesentiert
class LineareGleichung
{
	public:
		// Hilfsfunktionen
		LineareGleichung() : m1(0), n1(0), m2(0), n2(0) {}
		LineareGleichung(double u1, double j1, double u2, double j2) : m1(u1), n1(j1), m2(u2), n2(j2) {}

		// Ermittelt den Informationsgehalt
		// einer Gleichung und gibt diesen zurueck
		Infotyp bekommeInfoTyp()
		{
			if(m1 == 0 && m2 == 0)
			{
				return Infotyp::Keiner;
			}
			else if(m1 == 0 && m2 != 0)
			{
				return Infotyp::Rechts;
			}
			else if(m2 == 0 && m1 != 0)
			{
				return Infotyp::Links;
			}
			else if(m2 != 0 && m1 != 0)
			{
				return Infotyp::Beide;
			}
		}

		// Ermittelt die Wahrheit der Gleichung
		// insofern diese des Typs "Keine" ist
		bool istWahr()
		{
			return n1 == n2;
		}

		// Ermittelt die Wahrheit der Gleichung
		// insofern diese des Typs "Beide" ist
		// setzt fuer die beiden Variablen
		// bereits berechnete Werte ein
		bool istWahr(double l, double r)
		{
			return m1 * l + n1 == m2 * r + n2;
		}

		// Berechnet den Wert der linken Variable
		// insofern die Gleichung
		// des Typs "Links" ist
		double linksWert()
		{
			return (n2 - n1) / m1;
		}

		// Berechnet den Wert der rechten Variable
		// insofern die Gleichung
		// des Typs "Rechts" ist
		double rechtsWert()
		{
			return (n1 - n2) / m2;
		}

		// Setzt in die Gleichung die vorher berechnete
		// linke Variable ein, insofern die Gleichung des
		// Typs "Beide" ist, dabei aendert diese Gleichung
		// den Typ zu "Rechts" und der Wert der rechten Variable
		// wird zurueckgegeben
		double linksEinfuegen(double l)
		{
			n1 += m1 * l;
			m1 = 0;
			return rechtsWert();
		}

		// Setzt in die Gleichung die vorher berechnete
		// rechte Variable ein, insofern die Gleichung des
		// Typs "Beide" ist, dabei aendert diese Gleichung
		// den Typ zu "Links" und der Wert der linken Variable
		// wird zurueckgegeben
		double rechtsEinfuegen(double r)
		{
			n2 += m2 * r;
			m2 = 0;
			return linksWert();
		}

		// Setzt in die Gleichung, insofern dass diese
		// vom Typ "Beide" ist, eine andere des Typs "Beide"
		// ein. Zurueckgegeben werden die Werte fuer sowohl
		// die rechte als auch die linke Variable,
		// insofern es Sinn ergab
		Loesung einfuegen(LineareGleichung g)
		{
			Loesung l;

			l.loesbar = true;
			l.l = 0;
			l.r = 0;

			double a1 = 0;
			double b1 = 0;
			double c1 = 0;
			double d1 = 0;

			double a2 = 0;
			double b2 = 0;
			double c2 = 0;
			double d2 = 0;

			a1 = m1;
			b1 = n1;
			c1 = m2;
			d1 = n2;

			a2 = g.m1;
			b2 = g.n1;
			c2 = g.m2;
			d2 = g.n2;

			double nenner = 1 - a2 * c1 / (c2 * a1);

			if(nenner != 0)
			{
				l.r = (a2 * (d1 - b1) / a1 + b2 - d2) / (c2 * nenner);
				l.l = (c1 * l.r + d1 - b1) / a1;
				return l;
			}
			else
			{
				l.loesbar = false;
				return l;
			}
		}

		double m1;
		double m2;
		double n1;
		double n2;
};

// die Klasse Gleichungssystem (GS) ist im Grunde ein Feld von
// linearen Gleichungen, die außerdem in der Lage ist entsprechende
// Umformungen (teils) zu realisieren
// (der groessere Teil der Umformungen geschieht in der Klasse lineare Gleichung)
class Gleichungssystem
{
	public:
		// Fuegt eine Gleichung dem GS hinzu
		void fuegeGleichungHinzu(LineareGleichung g)
		{
			gleichungen.push_back(g);
		}

		// Loest das GS, durch
		// Loesung beinhaltet die Variablen r und l, wenn sinnvoll 
		Loesung loesen()
		{
			Loesung l;

			l.loesbar = true;
			l.r = 0;
			l.l = 0;
			bool rgesetzt = false;
			bool lgesetzt = false;

			for(int i = 0; i < gleichungen.size(); i++)
			{
				Infotyp aktuellerTyp = gleichungen.at(i).bekommeInfoTyp();
				if(aktuellerTyp == Infotyp::Keiner)
				{
					if(!gleichungen.at(i).istWahr())
					{
						l.loesbar = false;
						return l;
					}
				}
				else if(aktuellerTyp == Infotyp::Rechts)
				{
					if(rgesetzt)
					{
						if(l.r != gleichungen.at(i).rechtsWert())
						{
							l.loesbar = false;
							return l;
						}
					}
					else
					{
						l.r = gleichungen.at(i).rechtsWert();
						rgesetzt = true;
					}
				}
				else if(aktuellerTyp == Infotyp::Links)
				{
					if(lgesetzt)
					{
						if(l.l != gleichungen.at(i).linksWert())
						{
							l.loesbar = false;
							return l;
						}
					}
					else
					{
						l.l = gleichungen.at(i).linksWert();
						lgesetzt = true;
					}
				}

				if(aktuellerTyp != Infotyp::Beide)
				{
					gleichungen.erase(gleichungen.begin() + i--);
				}
			}

			for(int i = 0; i < gleichungen.size(); i++)
			{
				if(lgesetzt && rgesetzt)
				{
					if(!gleichungen.at(i).istWahr(l.l, l.r))
					{
						l.loesbar = false;
						return l;
					}
				}
				else if(lgesetzt)
				{
					l.r = gleichungen.at(i).linksEinfuegen(l.l);
					rgesetzt = true;
				}
				else if(rgesetzt)
				{
					l.l = gleichungen.at(i).linksEinfuegen(l.r);
					lgesetzt = true;
				}
				else
				{
					if(i + 1 == gleichungen.size())
					{
						l.loesbar = false;
						return l;
					}
					else
					{
						Loesung tempLoesung;
						tempLoesung = gleichungen.at(i).einfuegen(gleichungen.at(i + 1));
						i++;
						if(tempLoesung.loesbar)
						{
							l.r = tempLoesung.r;
							l.l = tempLoesung.l;
							lgesetzt = true;
							rgesetzt = true;
						}
						else
						{
							l.loesbar = false;
							return l;
						}
					}
				}
			}

			return l;
		}

	protected:
		std::vector<LineareGleichung> gleichungen;
};

// Die Klasse Gerade
// besteht im Wesentlichen aus 2 Vektoren,
// dem Stuetzvektor und dem Richtungsvektor
class Gerade
{
	public:
		Gerade() : Richtungsvektor(0, 0, 0) {}
		Gerade(Vektor s, Vektor r) : Stuetzvektor(s), Richtungsvektor(r) {}

		void printParameterform()
		{
			std::cout << Stuetzvektor.bekommePunktRepresentation() <<" + r * "<< Richtungsvektor.bekommePunktRepresentation() << std::endl;
		}

		// berechnet alle Spurpunkte
		// der Geraden und zeigt diese an
		void zeigeSpurpunktInfo()
		{
			double skalar = 0;
			Vektor spurpunkt;
			if(Richtungsvektor.x != 0)
			{
				skalar = -Stuetzvektor.x / Richtungsvektor.x;
				spurpunkt = Stuetzvektor + skalar * Richtungsvektor;
				std::cout <<"YZ"<< spurpunkt.bekommePunktRepresentation() << std::endl;
			}
			else if(Stuetzvektor.x == 0)
			{
				spurpunkt = Stuetzvektor;
				std::cout <<"YZ"<< spurpunkt.bekommePunktRepresentation() << std::endl;
			}
			else
			{
				std::cout <<"Parallel zur YZ-Ebene"<< std::endl;
			}

			if(Richtungsvektor.y != 0)
			{
				skalar = -Stuetzvektor.y / Richtungsvektor.y;
				spurpunkt = Stuetzvektor + skalar * Richtungsvektor;
				std::cout <<"XZ"<< spurpunkt.bekommePunktRepresentation() << std::endl;
			}
			else if(Stuetzvektor.y == 0)
			{
				spurpunkt = Stuetzvektor;
				std::cout <<"XZ"<< spurpunkt.bekommePunktRepresentation() << std::endl;
			}
			else
			{
				std::cout <<"Parallel zur XZ-Ebene"<< std::endl;
			}

			if(Richtungsvektor.z != 0)
			{
				skalar = -Stuetzvektor.z / Richtungsvektor.z;
				spurpunkt = Stuetzvektor + skalar * Richtungsvektor;
				std::cout <<"XY"<< spurpunkt.bekommePunktRepresentation() << std::endl;
			}
			else if(Stuetzvektor.z == 0)
			{
				spurpunkt = Stuetzvektor;
				std::cout <<"XY"<< spurpunkt.bekommePunktRepresentation() << std::endl;
			}
			else
			{
				std::cout <<"Parallel zur XY-Ebene"<< std::endl;
			}
		}

		// Hilfsfunktion zum Einlesen von Geraden
		bool einlesen(std::string input)
		{
			std::vector<std::string> parts = explode(';', input);
			if(parts.size() == 3)
			{
				if(parts.at(0) == "pa")
				{
					if(!(Stuetzvektor.parse(parts.at(1)) && Richtungsvektor.parse(parts.at(2))))
					{
						return false;
					}
				}
				else if(parts.at(0) == "pu")
				{
					if(Stuetzvektor.parse(parts.at(1)) && Richtungsvektor.parse(parts.at(2)))
					{
						Richtungsvektor = Richtungsvektor - Stuetzvektor;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}

			return true;
		}

		Vektor Stuetzvektor;
		Vektor Richtungsvektor;
};

// Die Klasse Ebene
// besteht aus 3 Vektoren
class Ebene
{
	public:
		Ebene() : Stuetzvektor(0, 0, 0), Richtungsvektor1(0, 0, 1), Richtungsvektor2(0, 1, 0) {}
		Ebene(Vektor a, Vektor b, Vektor c) : Stuetzvektor(a), Richtungsvektor1(b), Richtungsvektor2(c) {}
		~Ebene() {}

		void printKoordinatenform()
		{
			Vektor normale = Richtungsvektor1 % Richtungsvektor2;
			double d = normale * Stuetzvektor;
			bool isFirst = true;
			if(abs(normale.x) != 0)
			{
				if(normale.x < 0)
					std::cout <<" - ";
				if(abs(normale.x) != 1)
					std::cout << abs(normale.x);
				std::cout <<"x";
				isFirst = false;
			}

			if(abs(normale.y) != 0)
			{
				if(isFirst || normale.y > 0)
					std::cout <<" + ";
				else
					std::cout <<" - ";
				if(abs(normale.y) != 1)
					std::cout << abs(normale.y);
				std::cout <<"y";
				isFirst = false;
			}

			if(abs(normale.z) != 0)
			{
				if(isFirst || normale.z > 0)
					std::cout <<" + ";
				else
					std::cout <<" - ";
				if(abs(normale.z) != 1)
					std::cout << abs(normale.z);
				std::cout <<"z";
			}

			std::cout <<" = "<< d;

			std::cout << std::endl;
		}

		void printParameterform()
		{
			std::cout <<"X = ";
			if(Stuetzvektor != Vektor(0, 0, 0))
			{
				std::cout << Stuetzvektor.bekommePunktRepresentation() <<" + ";
			}
			std::cout <<"r * "<< Richtungsvektor1.bekommePunktRepresentation() <<" + s * "<< Richtungsvektor2.bekommePunktRepresentation() << std::endl;
		}

		void printNormalenform()
		{
			Vektor normale = Richtungsvektor1 % Richtungsvektor2;
			if(Stuetzvektor != Vektor(0, 0, 0))
			{
				std::cout <<"(X - "<< Stuetzvektor.bekommePunktRepresentation() <<") * "<< normale.bekommePunktRepresentation() <<" = 0"<< std::endl;
			}
			else
			{
				std::cout <<"X * "<< normale.bekommePunktRepresentation() <<" = 0"<< std::endl;
			}
		}

		Vektor bekommeNormalenvektor()
		{
			return Richtungsvektor1 % Richtungsvektor2;
		}

		bool einlesen(std::string input)
		{
			std::vector<std::string> parts;

			parts = explode(';', input);

			if(parts.size() > 1)
			{
				if(parts.at(0) == "pa")
				{
					if(parts.size() == 4)
					{
						if(!(Stuetzvektor.parse(parts.at(1)) && Richtungsvektor1.parse(parts.at(2)) && Richtungsvektor2.parse(parts.at(3))))
							return false;
						else
							return true;
					}
					else
						return false;
				}
				else if(parts.at(0) == "pu")
				{
					if(parts.size() == 4)
					{
						if(Stuetzvektor.parse(parts.at(1)) && Richtungsvektor1.parse(parts.at(2)) && Richtungsvektor2.parse(parts.at(3)))
						{
							Richtungsvektor1 = Stuetzvektor - Richtungsvektor1;
							Richtungsvektor2 = Stuetzvektor - Richtungsvektor2;
							return true;
						}
						else
							return false;
					}
					else
						return false;
				}
				else if(parts.at(0) == "no")
				{
					if(parts.size() == 3)
					{
						if(Stuetzvektor.parse(parts.at(1)) && Richtungsvektor1.parse(parts.at(2)))
						{
							std::vector<Vektor> veks = Richtungsvektor1.aufrollen();
							Richtungsvektor1 = veks[0];
							Richtungsvektor2 = veks[1];
							return true;
						}
						else
							return false;
					}
				}
				else if(parts.at(0) == "ko")
				{
					if(parts.size() == 2)
					{
						std::vector<std::string> gparts = explode({'+', '-'}, parts.at(1));

					}
					else
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}

		Vektor Richtungsvektor1;
		Vektor Richtungsvektor2;
		Vektor Stuetzvektor;
};

// Hilfsdatentyp der zusätzlich im Falle des Schneidens
// den Schnittpunkt bereits beinhaltet
typedef struct
{
	Vektor schnittpunkt;
	double schnittwinkel;
	double abstand;
	Gerade schnittgerade;
	Lage typ;
}
Lageinfo;

// Ermittelt Lagebeziehung und
// ggf. den Schnittpunkt zu einer
// anderen Gerade
Lageinfo lageZu(Gerade g1, Gerade g2)
{
	Lageinfo info;
	if(g1.Richtungsvektor.istKollinearZu(g2.Richtungsvektor))
	{
		info.abstand = (g1.Stuetzvektor + g1.Richtungsvektor * (((g2.Stuetzvektor - g1.Stuetzvektor) * g1.Richtungsvektor) / (g1.Richtungsvektor * g1.Richtungsvektor))).betrag();
		if((g1.Stuetzvektor - g2.Stuetzvektor).istKollinearZu(g2.Richtungsvektor))
		{
			info.typ = Lage::Identisch;
		}
		else
		{
			info.typ = Lage::Parallel;
		}
	}
	else
	{
		Gleichungssystem s;
		s.fuegeGleichungHinzu(LineareGleichung(g1.Richtungsvektor.x, g1.Stuetzvektor.x, g2.Richtungsvektor.x, g2.Stuetzvektor.x));
		s.fuegeGleichungHinzu(LineareGleichung(g1.Richtungsvektor.y, g1.Stuetzvektor.y, g2.Richtungsvektor.y, g2.Stuetzvektor.y));
		s.fuegeGleichungHinzu(LineareGleichung(g1.Richtungsvektor.z, g1.Stuetzvektor.z, g2.Richtungsvektor.z, g2.Stuetzvektor.z));

		Loesung loe = s.loesen();
		if(loe.loesbar)
		{
			info.typ = Lage::Schneiden;
			info.schnittpunkt = loe.l * g1.Richtungsvektor + g1.Stuetzvektor;
			info.schnittwinkel = g1.Richtungsvektor.bekommeSchnittwinkelZuRichtungsvektor(g2.Richtungsvektor);
			info.abstand = 0;
		}
		else
		{
			info.typ = Lage::Windschief;

			Vektor u, v, d1, d2;
			double r = 0, s = 0;

			d1 = g1.Stuetzvektor;
			d2 = g2.Stuetzvektor;
			u = g1.Richtungsvektor;
			v = g2.Richtungsvektor;

			s = ((d2 - d1 - (((d2 - d1) * u) / (u * u)) * u) * v) / (pow(v * u, 2) / (u * u) - (v * v));
			r = ((d2 - d1 + s * v) * u) / (u * u);

			info.abstand = (d1 - d2 + r * u - s * v).betrag();
		}
	}

	return info;
}

Lageinfo lageZu(Ebene e, Gerade g)
{
	Lageinfo info;

	Vektor normale = e.bekommeNormalenvektor();

	if(g.Richtungsvektor * normale == 0)
	{
		info.abstand = (g.Stuetzvektor - e.Stuetzvektor) * normale / normale.betrag();
		info.typ = info.abstand == 0 ? Lage::Liegend : Lage::Parallel;
	}
	else
	{
		info.typ = Lage::Schneiden;
		double r = (e.Stuetzvektor - g.Stuetzvektor) * normale / (g.Richtungsvektor * normale);

		info.schnittpunkt = g.Stuetzvektor + g.Richtungsvektor * r;
		info.schnittwinkel = g.Richtungsvektor.bekommeSchnittwinkelZuNormalenvektor(normale);
		info.abstand = 0;
	}

	return info;
}

Lageinfo lageZu(Ebene e1, Ebene e2)
{
	Lageinfo info;

	Vektor n1 = e1.bekommeNormalenvektor();
	Vektor n2 = e2.bekommeNormalenvektor();

	if(n1.istKollinearZu(n2))
	{
		info.abstand = myAbs((e1.Stuetzvektor - e2.Stuetzvektor) * n1 / n1.betrag());
		info.typ = info.abstand == 0 ? Lage::Identisch : Lage::Parallel;
	}
	else
	{
		info.typ = Lage::Schneiden;
		info.schnittwinkel = n1.bekommeSchnittwinkelZuRichtungsvektor(n2);
		new (&info.schnittgerade) Gerade(
			e2.Stuetzvektor + e1.Richtungsvektor1 * ((e1.Stuetzvektor - e2.Stuetzvektor) * n1) / (e2.Richtungsvektor1 * n1),
			e2.Richtungsvektor2 - e2.Richtungsvektor1 * ((e2.Richtungsvektor2 * n1) / (e2.Richtungsvektor1 * n1))
		);
	}

	return info;
}

// Einstiegspunkt des Programms
int main()
{
	// Variablendeklarationen
	std::vector<Gerade> geraden;
	std::vector<Ebene> ebenen;
	std::vector<std::string> inp;
	std::vector<std::string> gnames;
	std::vector<std::string> enames;
	std::string tline = "";
	Lageinfo linfo;
	int counter = 0;
	Gerade tGerade;
	Ebene tEbene;
	bool debug = true;

	if(debug)
	{
		// Automatische Testeingaben anfang
		std::string ginputs[] =
		{
			"g1:pa;(0,1,2);(2,1,-3)",
			"h1:pa;(-2,-2,7);(-2,1,1)",
			"g2:pa;(1,1,2);(1,-2,2)",
			"h2:pa;(-1,2,1);(-2,4,-4)",
			"g3:pa;(3,0,1);(1,1,-2)",
			"h3:pa;(0,2,0);(2,1,1)",
			"g4:pa;(2,0,1);(2,1,-1)",
			"h4:pa;(0,2,-4);(2,0,1)",
			"k1:pa;(7,4,3);(2,0,1)"
		};

		std::string einputs[] =
		{
			"E1:pa;(0,0,1);(3,5,0);(-3,0,7)",
			"E2:pu;(3,5,7);(-1,0,1);(4,5,0)",
			"E3:no;(7,4,3);(0,3,0)",
			"E4:pa;(0,3,4);(0,1,0);(1,0,0)",
			"YZ:no;(0,0,0);(1,0,0)",
			"XZ:no;(0,0,0);(0,1,0)",
			"I1:pa;(0,0,0);(3,0,2);(-2,0,0)",
			"I2:no;(0,0,0);(0,1,0)"
		};

		for(int i = 0; i < 9; i++)
		{
			inp = explode(':', ginputs[i]);
			if(inp.size() == 2)
			{
				if(tGerade.einlesen(inp.at(1)))
				{
					gnames.push_back(inp.at(0));
					geraden.push_back(tGerade);
				}
			}
		}	

		for(int i = 0; i < 8; i++)
		{
			inp = explode(':', einputs[i]);
			if(inp.size() == 2)
			{
				if(tEbene.einlesen(inp.at(1)))
				{
					enames.push_back(inp.at(0));
					ebenen.push_back(tEbene);
				}
			}
		}

		// Automatische Testeingaben ende
	}
	else
	{
		// Eingaben des Nutzers anfang

		std::cout <<"Ebenen:"<< std::endl;
		counter = 1;
		do
		{
			std::cout << counter <<". ";
			std::getline(std::cin, tline);
			inp = explode(':', tline);
			if(inp.size() == 2)
			{
				if(tEbene.einlesen(inp.at(1)))
				{
					while(in_array(inp.at(0), enames))
					{
						std::cout <<"Der Name ist schon vergeben, neuer Name: ";
						std::getline(std::cin, inp.at(0));
					}
					enames.push_back(inp.at(0));
					ebenen.push_back(tEbene);
					counter++;
				}
				else
					std::cout <<"Falsche Eingabe"<< std::endl;
			}
			else if((inp.size() == 1 && inp.at(0) != "fertig") || inp.size() != 1)
				std::cout <<"Falsche Eingabe"<< std::endl;
		}
		while(!(inp.size() == 1 && inp.at(0) == "fertig"));
		
		std::cout <<"Geraden:"<< std::endl;
		counter = 1;
		do
		{
			std::cout << counter <<". ";
			std::getline(std::cin, tline);
			inp = explode(':', tline);
			if(inp.size() == 2)
			{
				if(tGerade.einlesen(inp.at(1)))
				{
					while(in_array(inp.at(0), gnames))
					{
						std::cout <<"Der Name ist schon vergeben, neuer Name: ";
						std::getline(std::cin, inp.at(0));
					}
					gnames.push_back(inp.at(0));
					geraden.push_back(tGerade);
					counter++;
				}
				else
					std::cout <<"Falsche Eingabe"<< std::endl;
			}
			else if((inp.size() == 1 && inp.at(0) != "fertig") || inp.size() != 1)
				std::cout <<"Falsche Eingabe"<< std::endl;
		}
		while(!(inp.size() == 1 && inp.at(0) == "fertig"));

		// Eingaben des Nutzers ende
	}

	std::cout << std::endl;

	// Berechnung und Ausgaben

	if(enames.size() == ebenen.size() && gnames.size() == geraden.size())
	{
		std::cout <<"Spurpunkte:"<< std::endl;
		for(int i = 0; i < geraden.size(); i++)
		{
			std::cout << gnames.at(i) <<":"<< std::endl;
			geraden.at(i).zeigeSpurpunktInfo();
			std::cout << std::endl;
		}

		for(int i = 0; i < geraden.size() - 1; i++)
		{
			for(int p = i + 1; p < geraden.size(); p++)
			{
				linfo = lageZu(geraden.at(i), geraden.at(p));
				std::cout << gnames.at(i) <<" zu "<< gnames.at(p) <<": ";
				switch(linfo.typ)
				{
					case Lage::Identisch:
						std::cout <<"identisch"<< std::endl;
					break;
					case Lage::Parallel:
						std::cout <<"parallel im Abstand von "<< fpff(linfo.abstand) << std::endl;
					break;
					case Lage::Windschief:
						std::cout <<"windschief im Abstand von "<< fpff(linfo.abstand) << std::endl;
					break;
					case Lage::Schneiden:
						if(linfo.schnittwinkel >= 90.f)
							linfo.schnittwinkel -= 90.f;
						std::cout <<"schneiden in Punkt"<< linfo.schnittpunkt.bekommePunktRepresentation() <<" unter einem Winkel von "<< fpff(linfo.schnittwinkel) <<" Grad"<< std::endl;
					break;
					default:
						std::cout<<"Default"<< std::endl;
					break;
				}
			}
		}

		for(int p = 0; p < geraden.size(); p++)
		{
			for(int i = 0; i < ebenen.size(); i++)
			{
				linfo = lageZu(ebenen.at(i), geraden.at(p));

				switch(linfo.typ)
				{
					case Lage::Schneiden:
						std::cout <<"Gerade "<< gnames.at(p) <<" durchstoesst Ebene "<< enames.at(i) <<" im Punkt "<< linfo.schnittpunkt.bekommePunktRepresentation() <<" und schliesst einen Winkel von "<< fpff(myAbs(linfo.schnittwinkel)) <<" Grad ein"<< std::endl;
					break;
					case Lage::Parallel:
						std::cout <<"Gerade "<< gnames.at(p) <<" ist echtparallel zu Ebene "<< enames.at(i) <<" im Abstand von "<< linfo.abstand << std::endl;
					break;
					case Lage::Liegend:
						std::cout <<"Gerade "<< gnames.at(p) <<" liegt in der Ebene "<< enames.at(i) << std::endl;
					break;
					default:
						std::cout<<"Default"<< std::endl;
					break;
				}
			}
		}

		for(int i = 0; i < ebenen.size() - 1; i++)
		{
			for(int p = i + 1; p < ebenen.size(); p++)
			{
				linfo = lageZu(ebenen.at(i), ebenen.at(p));

				switch(linfo.typ)
				{
					case Lage::Identisch:
						std::cout <<"Ebene "<< enames.at(p) <<" ist identisch zu Ebene "<< enames.at(i) << std::endl;
					break;
					case Lage::Parallel:
						std::cout <<"Ebene "<< enames.at(p) <<" liegt echtparallel zu Ebene "<< enames.at(i) <<" im Abstand von "<< fpff(linfo.abstand) << std::endl;
					break;
					case Lage::Schneiden:
						std::cout <<"Ebene "<< enames.at(p) <<" schneidet Ebene "<< enames.at(i) <<" unter einem Winkel von "<< fpff(linfo.schnittwinkel) <<" Grad in der Geraden ";
						linfo.schnittgerade.printParameterform();
					break;
					default:
						std::cout<<"Default"<< std::endl;
					break;
				} 
			}
		}
	}
	else
		std::cout <<"Fehler der Feldergroessen (sollte eigentlich nicht passieren)"<< std::endl;

	// Berechnungen und Ausgaben ende

	return 0;
}