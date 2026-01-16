#ifndef NAMEDAYS_H
#define NAMEDAYS_H

// Slovak name days (meniny) lookup table - ASCII only (no diacritics)
// Index: [month 0-11][day 0-30]
const char* const NAME_DAYS[12][31] = {
    // January (0)
    {"Novy rok", "Alexandra", "Daniela", "Drahoslav", "Andrea", "Antonia", "Bohuslava",
     "Severin", "Alexej", "Dasa", "Malvina", "Ernest", "Rastislav", "Radovan",
     "Dobroslav", "Kristina", "Natasa", "Bohdana", "Drahomira", "Dalibor", "Vincent",
     "Zora", "Milos", "Timotej", "Gejza", "Tamara", "Bohus", "Alfonz", "Gaspar",
     "Ema", "Emil"},
    // February (1)
    {"Tatiana", "Erik", "Blazej", "Veronika", "Agata", "Dorota", "Vanda", "Zoja",
     "Zdenko", "Gabriela", "Dezider", "Perla", "Arpad", "Valentin", "Pravoslav",
     "Ida Liana", "Miloslava", "Jaromir", "Vlasta", "Livia", "Eleonora", "Etela",
     "Roman", "Matej", "Frederik", "Viktor", "Alexander", "Zlatica", "Radomir",
     "", ""},
    // March (2)
    {"Albin", "Anezka", "Bohumil", "Kazimir", "Fridrich", "Radoslav", "Tomas",
     "Alan", "Frantiska", "Branislav", "Angela", "Gregor", "Vlastimil", "Matilda",
     "Svetlana", "Boleslav", "Lubica", "Eduard", "Jozef", "Vitazoslav", "Blahoslav",
     "Benadik", "Adrian", "Gabriel", "Marian", "Emanuel", "Alena", "Sona",
     "Miroslav", "Vieroslava", "Benjamin"},
    // April (3)
    {"Hugo", "Zita", "Richard", "Izidor", "Miroslava", "Irena", "Zoltan", "Albert",
     "Milena", "Igor", "Julius", "Estera", "Ales", "Justina", "Fedor", "Dana",
     "Rudolf", "Valer", "Jela", "Marcel", "Ervin", "Slavomir", "Vojtech", "Juraj",
     "Marek", "Jaroslava", "Jaroslav", "Jarmila", "Lea", "Anastazia", ""},
    // May (4)
    {"Sviatok prace", "Zigmund", "Galina", "Florian", "Lesana", "Hermina", "Monika",
     "Ingrida", "Roland", "Viktoria", "Blazena", "Pankrac", "Servac", "Bonifac",
     "Zofia", "Svetozar", "Gizela", "Viola", "Gertruda", "Bernard", "Zina", "Julia",
     "Zelmira", "Ela", "Urban", "Dusan", "Iveta", "Viliam", "Vilma", "Ferdinand",
     "Petronela"},
    // June (5)
    {"Zaneta", "Xenia", "Karolina", "Lenka", "Laura", "Norbert", "Robert", "Medard",
     "Stanislava", "Margareta", "Dobroslava", "Zlatko", "Anton", "Vasil", "Vit",
     "Blanka", "Adolf", "Vratislav", "Alfred", "Valeria", "Alojz", "Paulina",
     "Sidonia", "Jan", "Tadeas", "Adriana", "Ladislav", "Beata", "Peter", "Melania",
     ""},
    // July (6)
    {"Diana", "Berta", "Miloslav", "Prokop", "Cyril", "Patrik", "Oliver", "Ivan",
     "Lujza", "Amalia", "Milota", "Nina", "Margita", "Kamil", "Henrich", "Drahomir",
     "Bohuslav", "Kamila", "Dusana", "Ilja", "Daniel", "Magdalena", "Olga",
     "Vladimir", "Jakub", "Anna Hana", "Bozena", "Kristof", "Marta", "Libusa",
     "Ignac"},
    // August (7)
    {"Bozidara", "Gustav", "Jergus", "Dominik", "Hortenzia", "Jozefina", "Stefania",
     "Oskar", "Lubomira", "Vavrinec", "Zuzana", "Darina", "Lubomir", "Mojmir",
     "Marcela", "Leonard", "Milica", "Elena Helena", "Lydia", "Anabela", "Jana",
     "Tichomir", "Filip", "Bartolomej", "Ludovit", "Samuel", "Silvia", "Augustin",
     "Nikola", "Ruzena", "Nora"},
    // September (8)
    {"Drahoslava", "Linda", "Belo", "Rozalia", "Regina", "Alica", "Marianna",
     "Miriama", "Martina", "Oleg", "Bystrik", "Maria", "Ctibor", "Ludomil", "Jolana",
     "Ludomila", "Olympia", "Eugenia", "Konstantin", "Luboslav", "Matus", "Moric",
     "Zdenka", "Lubos", "Vladislav", "Edita", "Cyprian", "Vaclav", "Michal",
     "Jarolim", ""},
    // October (9)
    {"Arnold", "Levoslav", "Stela", "Frantisek", "Viera", "Natalia", "Eliska",
     "Brigita", "Dionyz", "Slavomira", "Valentina", "Maximilian", "Koloman", "Boris",
     "Terezia", "Vladimira", "Hedviga", "Lukas", "Kristian", "Vendelin", "Ursula",
     "Sergej", "Alojza", "Kvetoslava", "Aurel", "Demeter", "Sabina", "Dobromila",
     "Klara", "Simona", "Aurelia"},
    // November (10)
    {"Denis Denisa", "Pamiatka zosnulych", "Hubert", "Karol", "Imrich", "Renata",
     "Rene", "Bohumir", "Teodor", "Tibor", "Maros Martin", "Svatopluk", "Stanislav",
     "Irma", "Leopold", "Agnesa", "Klaudia", "Eugen", "Alzbeta", "Felix", "Elvira",
     "Cecilia", "Klement", "Emilia", "Katarina", "Kornel", "Milan", "Henrieta",
     "Vratko", "Ondrej", ""},
    // December (11)
    {"Edmund", "Bibiana", "Oldrich", "Barbora", "Oto", "Mikulas", "Ambroz", "Marina",
     "Izabela", "Raduz", "Hilda", "Otilia", "Lucia", "Branislava", "Ivica", "Albina",
     "Kornelia", "Slava", "Judita", "Dagmara", "Bohdan", "Adela", "Nadezda", "Adam",
     "1.sviatok vianocny", "Stefan", "Filomena", "Ivana", "Milada", "David",
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
