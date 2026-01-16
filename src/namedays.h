#ifndef NAMEDAYS_H
#define NAMEDAYS_H

// Slovak name days (meniny) lookup table
// Index: [month 0-11][day 0-30]
const char* const NAME_DAYS[12][31] = {
    // January (0)
    {"Nový rok", "Alexandra", "Daniela", "Drahoslav", "Andrea", "Antónia", "Bohuslava",
     "Severín", "Alexej", "Dáša", "Malvína", "Ernest", "Rastislav", "Radovan",
     "Dobroslav", "Kristína", "Nataša", "Bohdana", "Drahomíra", "Dalibor", "Vincent",
     "Zora", "Miloš", "Timotej", "Gejza", "Tamara", "Bohuš", "Alfonz", "Gašpar",
     "Ema", "Emil"},
    // February (1)
    {"Tatiana", "Erik", "Blažej", "Veronika", "Agáta", "Dorota", "Vanda", "Zoja",
     "Zdenko", "Gabriela", "Dezider", "Perla", "Arpád", "Valentín", "Pravoslav",
     "Ida Liana", "Miloslava", "Jaromír", "Vlasta", "Lívia", "Eleonóra", "Etela",
     "Roman", "Matej", "Frederik", "Viktor", "Alexander", "Zlatica", "Radomír",
     "", ""},
    // March (2)
    {"Albín", "Anežka", "Bohumil", "Kazimír", "Fridrich", "Radoslav", "Tomáš",
     "Alan", "Františka", "Branislav", "Angela", "Gregor", "Vlastimil", "Matilda",
     "Svetlana", "Boleslav", "Ľubica", "Eduard", "Jozef", "Víťazoslav", "Blahoslav",
     "Beňadik", "Adrián", "Gabriel", "Marián", "Emanuel", "Alena", "Soňa",
     "Miroslav", "Vieroslava", "Benjamín"},
    // April (3)
    {"Hugo", "Zita", "Richard", "Izidor", "Miroslava", "Irena", "Zoltán", "Albert",
     "Milena", "Igor", "Július", "Estera", "Aleš", "Justína", "Fedor", "Dana",
     "Rudolf", "Valér", "Jela", "Marcel", "Ervín", "Slavomír", "Vojtech", "Juraj",
     "Marek", "Jaroslava", "Jaroslav", "Jarmila", "Lea", "Anastázia", ""},
    // May (4)
    {"Sviatok práce", "Žigmund", "Galina", "Florián", "Lesana", "Hermína", "Monika",
     "Ingrida", "Roland", "Viktória", "Blažena", "Pankrác", "Servác", "Bonifác",
     "Žofia", "Svetozár", "Gizela", "Viola", "Gertrúda", "Bernard", "Zina", "Júlia",
     "Želmíra", "Ela", "Urban", "Dušan", "Iveta", "Viliam", "Vilma", "Ferdinand",
     "Petronela"},
    // June (5)
    {"Žaneta", "Xénia", "Karolína", "Lenka", "Laura", "Norbert", "Róbert", "Medard",
     "Stanislava", "Margaréta", "Dobroslava", "Zlatko", "Anton", "Vasil", "Vít",
     "Blanka", "Adolf", "Vratislav", "Alfréd", "Valéria", "Alojz", "Paulína",
     "Sidónia", "Ján", "Tadeáš", "Adriána", "Ladislav", "Beáta", "Peter", "Melánia",
     ""},
    // July (6)
    {"Diana", "Berta", "Miloslav", "Prokop", "Cyril", "Patrik", "Oliver", "Ivan",
     "Lujza", "Amália", "Milota", "Nina", "Margita", "Kamil", "Henrich", "Drahomír",
     "Bohuslav", "Kamila", "Dušana", "Iľja", "Daniel", "Magdaléna", "Oľga",
     "Vladimír", "Jakub", "Anna Hana", "Božena", "Krištof", "Marta", "Libuša",
     "Ignác"},
    // August (7)
    {"Božidara", "Gustáv", "Jerguš", "Dominik", "Hortenzia", "Jozefína", "Štefánia",
     "Oskar", "Ľubomíra", "Vavrinec", "Zuzana", "Darina", "Ľubomír", "Mojmír",
     "Marcela", "Leonard", "Milica", "Elena Helena", "Lýdia", "Anabela", "Jana",
     "Tichomír", "Filip", "Bartolomej", "Ľudovít", "Samuel", "Silvia", "Augustín",
     "Nikola", "Ružena", "Nora"},
    // September (8)
    {"Drahoslava", "Linda", "Belo", "Rozália", "Regína", "Alica", "Marianna",
     "Miriama", "Martina", "Oleg", "Bystrík", "Mária", "Ctibor", "Ľudomil", "Jolana",
     "Ľudomila", "Olympia", "Eugénia", "Konštantín", "Ľuboslav", "Matúš", "Móric",
     "Zdenka", "Ľuboš", "Vladislav", "Edita", "Cyprián", "Václav", "Michal",
     "Jarolím", ""},
    // October (9)
    {"Arnold", "Levoslav", "Stela", "František", "Viera", "Natália", "Eliška",
     "Brigita", "Dionýz", "Slavomíra", "Valentína", "Maximilián", "Koloman", "Boris",
     "Terézia", "Vladimíra", "Hedviga", "Lukáš", "Kristián", "Vendelín", "Uršuľa",
     "Sergej", "Alojza", "Kvetoslava", "Aurel", "Demeter", "Sabína", "Dobromila",
     "Klára", "Simona", "Aurélia"},
    // November (10)
    {"Denis Denisa", "Pamiatka zosnulých", "Hubert", "Karol", "Imrich", "Renáta",
     "René", "Bohumír", "Teodor", "Tibor", "Maroš Martin", "Svätopluk", "Stanislav",
     "Irma", "Leopold", "Agnesa", "Klaudia", "Eugen", "Alžbeta", "Félix", "Elvíra",
     "Cecília", "Klement", "Emília", "Katarína", "Kornel", "Milan", "Henrieta",
     "Vratko", "Ondrej", ""},
    // December (11)
    {"Edmund", "Bibiána", "Oldrich", "Barbora", "Oto", "Mikuláš", "Ambróz", "Marína",
     "Izabela", "Radúz", "Hilda", "Otília", "Lucia", "Branislava", "Ivica", "Albína",
     "Kornélia", "Sláva", "Judita", "Dagmara", "Bohdan", "Adela", "Nadežda", "Adam",
     "1.sviatok vianočný", "Štefan", "Filoména", "Ivana", "Milada", "Dávid",
     "Silvester"}
};

// Get name day for given month (1-12) and day (1-31)
inline const char* getNameDay(int month, int day) {
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return "";
    }
    return NAME_DAYS[month - 1][day - 1];
}

#endif // NAMEDAYS_H
