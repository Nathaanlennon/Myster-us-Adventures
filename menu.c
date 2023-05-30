#include <stdio.h>

#include "include/macro.h"
#include "include/game.h"
#include "include/scoremanager.h"
#include "include/macro.h"
#include "include/usual.h"

//ouvrir et afficher le fichier des scores
void open_scores(){
    if(printFile("scores.txt") == 0){ // si il n'y a pas encore de fichier des scores car personne n'a jamais joué
        printf("Mais personne n'a jamais joué encore... Retournons au menu.\n");
    }
    else{
        printFile("scores.txt");
    }
    waiting();
}

//texte récurrent du menu tuto
void tuto_menu_text() {
    clear_all();//vide le terminal pour propreté
    printf("%sBienvenue dans l'incroyable %sTutoriel%s !\n", C_WHT, C_YEL, C_WHT);
    printf("\n\nC'est très simple, ici nous allons parler des règles et des différents éléments\n");
    printf("\n\n%s[1] Objectif\n"
           "%s[2] Les personnages\n"
           "%s[3] Le systeme de combat\n"
           "%s[4] Les monstres\n"
           "%s[5] Les objets\n"
           "%s[6] Retourner au menu principal%s\n", C_YEL, C_GRN, C_BLU, C_PUE, C_MAG, C_RED, C_WHT);
}

//texte récurrent du menu objet
void object_menu_text(){
    clear_all();//vide le terminal pour propreté
    printf("%sLes objets : \n"
           "%s[1] l'épée flamboyante %s\n"
           "%s[2] la dague du sommeil %s\n"
           "%s[3] le grimoire interdit %s\n"
           "%s[4] le baton de controle des familiers %s\n"
           "%s[5] le trésor %s\n"
           "%s[6] le totem de transmutation %s\n"
           "%s[7] le portail magique %s\n"
           "%s[8] retour vers le menu du tutoriel%s\n",
           C_MAG, C_ORE,SWORD, C_LGR,DAGGER, C_LBE, SPELLBOOK, C_GRN, STICK, C_YEL, CHEST, C_MAG, TOTEM, C_MAG,PORTAL,C_RED, C_WHT);
}
void object_menu(){
    char choice;
    object_menu_text();//texte récurrent menu objet
    while ((choice = getchar()) != EOF) {
        discardInput();//vider le buffer
        clear_all();//clear l'ecran pour que ça soit propre
        switch (choice) {
            case '1':
                printf("%sL'épée flamboyante : %s\nAu temps où les dieux nordiques marchaient parmi les hommes, une "
                       "légende s'éleva autour de l'épée légendaire nommée Eldhjärta. Cette arme d'une beauté "
                       "transcendante était forgée dans les flammes divines elles-mêmes.\n\nSelon les récits ancestraux, "
                       "Eldhjärta était la manifestation vivante du feu primordial, capturant l'énergie ardente qui "
                       "embrasait les cœurs et animait l'univers. Elle était l'épée des héros intrépides, choisis par les "
                       "dieux pour accomplir des quêtes héroïques et défendre les royaumes des forces obscures.\n\nLa légende "
                       "raconte qu'Eldhjärta était dotée d'une conscience propre et qu'elle choisissait elle-même ceux qui "
                       "étaient dignes de la brandir. Les élus ressentaient la chaleur du pouvoir flamboyant qui émanait "
                       "de la lame, et leur cœur s'embrasait d'une détermination inébranlable. \n\nQuiconque maniait "
                       "Eldhjärta se voyait investi d'une force surhumaine, capable de réduire les ennemis les plus "
                       "redoutables en cendres. L'épée guidait son porteur dans les batailles les plus féroces, "
                       "illuminant les ténèbres de sa lueur embrasée et instillant la peur dans le cœur des adversaires. "
                       "\nMais Eldhjärta n'était pas qu'une arme de destruction. Elle symbolisait aussi la volonté ardente "
                       "de préserver l'équilibre entre les forces du bien et du mal. Son porteur était appelé à user de "
                       "son pouvoir avec sagesse et compassion, car la flamme qui brûlait en Eldhjärta ne devait jamais "
                       "être corrompue par des intentions malveillantes. \n\nLa légende de Eldhjärta perdura à travers les âges, "
                       "transmise de génération en génération, et devint le symbole de l'héroïsme et de la passion. Même "
                       "si l'épée elle-même disparut dans les brumes du temps, son souvenir brûle encore dans les cœurs "
                       "des hommes, rappelant le pouvoir du feu sacré et la capacité des mortels à embraser le monde de "
                       "leur bravoure.\n", C_ORE, C_WHT);
                waiting();
                break;

            case '2':
                printf("%sLa dague du sommeil :%s\n", C_LGR, C_WHT);
                printf("Dans les récits oubliés de l'ère ancienne, une légende mystérieuse se tisse autour de la "
                       "Dague de Sommeil. Cette arme maléfique, aux origines nébuleuses, était réputée pour son pouvoir "
                       "ensorcelant qui plongeait ses victimes dans un sommeil éternel.\n\nOn dit que la Dague de Sommeil "
                       "était autrefois la possession d'un sombre sorcier qui maîtrisait les arts occultes. La lame, "
                       "forgée dans les ténèbres les plus profondes, était imprégnée d'un enchantement sinistre. "
                       "Quiconque était touché par cette dague maudite voyait ses paupières se fermer à jamais, plongeant "
                       "son esprit dans un sommeil sans fin.\n\nL'histoire raconte que la Dague de Sommeil fut utilisée par "
                       "le sorcier pour semer le chaos et la terreur. Ses ennemis étaient frappés par surprise, leurs "
                       "forces anéanties par le pouvoir somnifère de l'arme maudite. Les royaumes furent plongés dans une "
                       "torpeur effrayante, incapables de se défendre contre les attaques sournoises de ceux qui "
                       "brandissaient la dague.\n\nCependant, la Dague de Sommeil n'était pas sans sa propre malédiction. "
                       "Quiconque osait la tenir risquait de tomber sous l'emprise de son enchantement. Les légendes "
                       "rapportent que ceux qui la portaient devenaient esclaves de la torpeur éternelle, condamnés à "
                       "errer dans un état de sommeil profond jusqu'à la fin des temps.\n\nAinsi, la Dague de Sommeil devint "
                       "un artefact redouté et convoité. Certains cherchaient à la détruire, espérant briser son pouvoir "
                       "néfaste, tandis que d'autres, assoiffés de pouvoir, désiraient la posséder pour leur propre "
                       "domination. Quel que soit le destin de cette dague maudite, sa légende persiste, avertissant les "
                       "imprudents des dangers qui les guettent dans les ombres du sommeil éternel.\n\n");
                waiting();
                break;

            case '3':
                printf("%sLe grimmoire intedit :%s\n", C_LBE, C_WHT);
                printf("Au sein des antiques bibliothèques occultes, un sombre secret demeure enfoui : le Grimoire "
                       "Interdit. Cet ouvrage maudit, réputé pour renfermer des connaissances interdites et des sorts "
                       "impies, a été perdu dans les méandres du temps.\n\nSelon les légendes obscures, le Grimoire Interdit "
                       "aurait été écrit par une entité maléfique, liée aux ténèbres les plus profondes. Ses pages jaunies "
                       "par le temps sont couvertes d'écrits envoûtants, dont la signification échappe à ceux qui n'ont "
                       "pas été initiés aux mystères occultes.\n\nOn dit que ce grimoire renferme des formules de magie "
                       "noire d'une puissance incommensurable, capables de déchaîner des forces démoniaques et d'invoquer "
                       "des entités des abîmes les plus sombres. Les connaissances interdites qu'il contient pourraient "
                       "corrompre l'esprit des imprudents et les plonger dans une folie insondable.\n\nPourtant, malgré "
                       "sa sinistre réputation, le Grimoire Interdit a été perdu dans les annales du temps. Sa localisation "
                       "est devenue un mystère, cachée dans les recoins sombres de l'histoire oubliée. Certains prétendent "
                       "qu'il est dissimulé dans un lieu inconnu, gardé par des gardiens obscurs, tandis que d'autres "
                       "pensent qu'il a été englouti par les ombres pour toujours.\n\nLa légende du Grimoire Interdit "
                       "fascine et effraie les esprits curieux depuis des générations. Certains rêvent de le posséder, "
                       "espérant obtenir un pouvoir insoupçonné, tandis que d'autres le craignent, redoutant les "
                       "conséquences d'une telle connaissance.\n\nQuels que soient la vérité et le sort réel du Grimoire "
                       "Interdit, son nom persiste dans les murmures des initiés et les cauchemars des âmes tourmentées. "
                       "Une relique perdue, mais dont les mystères et les dangers continuent de hanter l'imagination des "
                       "hommes.\n");
                waiting();
                break;

            case '4':
                printf("%sLe baton de controle des familiers : %s\n", C_GRN, C_WHT);
                printf("Au cœur des terres sauvages, un artefact mystique connu sous le nom de Bâton de Contrôle "
                       "des Familiers éveille l'admiration des rôdeurs et des adeptes de la magie des créatures. Ce bâton, "
                       "taillé dans un bois ancien imprégné d'énergies magiques, confère à son possesseur le pouvoir de "
                       "communier avec les créatures magiques et de les diriger à sa volonté.\n\nLes rôdeurs, gardiens des "
                       "forêts et des contrées sauvages, ont longtemps recherché ce bâton légendaire pour renforcer leur "
                       "lien avec les créatures qui peuplent ces territoires. Ils croient en l'harmonie entre les mondes "
                       "humain et naturel, et voient en ce bâton un moyen de renforcer cette connexion sacrée.\n\nGrâce "
                       "au Bâton de Contrôle des Familiers, le rôdeur peut établir un lien empathique avec les créatures "
                       "magiques, ressentant leurs émotions et communiquant avec elles de manière instinctive. Les familiers, "
                       "en retour, répondent à l'appel du rôdeur et lui prêtent leur force et leurs pouvoirs magiques.\n"
                       "\nCependant, l'utilisation de ce bâton demande une grande responsabilité. Les rôdeurs doivent "
                       "honorer leur serment de protéger et de respecter la nature, utilisant leur pouvoir sur les "
                       "créatures magiques avec sagesse et discernement. Le Bâton de Contrôle des Familiers est un outil "
                       "sacré, dont l'équilibre entre l'homme et la nature doit être préservé.\n\nAu fil des âges, le "
                       "Bâton de Contrôle des Familiers est devenu un symbole de l'union entre les rôdeurs et les créatures "
                       "magiques. Il incarne la sagesse de ceux qui marchent parmi les arbres anciens, la force des liens "
                       "tissés entre les mondes et la confiance mutuelle entre le rôdeur et ses familiers.\n\nAujourd'hui "
                       "encore, les rôdeurs intrépides se lancent dans des quêtes pour retrouver le Bâton de Contrôle des "
                       "Familiers, espérant renforcer leur connexion avec les créatures magiques et préserver l'harmonie "
                       "entre les mondes. Cet artefact magique demeure un trésor convoité, gardien de la magie sauvage et "
                       "de la sagesse ancestrale des rôdeurs.\n");
                waiting();
                break;

            case '5':
                printf("%sLe trésor :%s\n", C_YEL, C_WHT);
                printf("Le trésor … Lorsque celui-ci est ouvert, on peut y voir tout ce dont on a toujours rêvé. "
                       "Une montagne d’or, des diamants, des bijoux … mais surtout un miroir. Un miroir qui a pour "
                       "particularité d’exaucer n'importe quel voeu. Tout ce dont vous avez toujours souhaité pourrait "
                       "devenir réel, ces rêves deviendraient réalité. Votre vie pourrait changer pour le mieux… Enfin, "
                       "cela dépendra de vos choix. Mais attention chers aventuriers vous n’avez le droit qu’à trois voeux "
                       "et ceux-là pourraient avoir de lourdes conséquences.\n");
                waiting();
                break;

            case '6':
                printf("%sLe totem de transmutation :%s\n", C_MAG, C_WHT);
                printf("Le totem de transmutation est un artefact magique mystérieux dont la force magique permet "
                       "de modifier la structure même d'un donjon. Cependant, de part son caractère mystérieux et le fait "
                       "qu'il modifie la configuration du dit donjon, il est necessaire à l'aventurier de recommencer son "
                       "exploration depuis le début\n");
                waiting();
                break;

            case '7':
                printf("%sLe portail magique :%s.\n", C_MAG,C_WHT);
                printf("Né d'une magie arcanique, le portail magique permet à son utilisateur de se téléporter "
                       "n'importe où dans un donjon. Cela peut être un atout comme un malus, alors à vous de décider si "
                       "vous souhaitez l'emprunter...\n");
                waiting();
                break;
            case '8':
                return;//quitter le sous menu objet pour revenir dans le menu tutoriel
            default:
                //rien par défaut
                break;
        }
        object_menu_text();//texte récurrent
    }
}

//le menu du tuto, il comporte toutes les explications du jeu
void tuto_menu() {
    tuto_menu_text();//le texte récurrent du menu
    int choice;
    while ((choice = getchar()) != EOF) {
        discardInput();//vider le buffer
        switch (choice) {
            case '1':
                printf("%sL'objectif :%s\n", C_YEL, C_WHT);
                printf("L'objectif ici est pour chaque joueur de récupérer son arme antique et le trésor.\n"
                       "La partie se déroule comme suit : quand un joueur avance, il dévoile les cases une par une jusqu'à ce qu'il perde un combat ou se retrouve bloqué (ne peut plus avancer).\n"
                       "Quand cela arrive, les cartes sont retournées face cachée et c'est au tour du joueur suivant.\n"
                       "Le premier qui réussi cette quête remporte la partie.\n");
                waiting();
                break;
            case '2':
                printf("%sLes personnages :%s\n", C_GRN, C_WHT);
                printf("Ils sont au nombre de 4 aventuriers :\n");
                printf("%sLe ranger : %sLe Ranger est un aventurier à la recherche constante de trésors. "
                       "Lorsqu’il entendu parler d’un donjon contenant de nombreuses merveilleux, il ne put contenir son "
                       "enthousiasme. C’est un homme qui vagabonde et explore tous les coins du monde. Il apprit la "
                       "survie de lui-même et il sut cohabiter avec les animaux sauvages. Il recherche %sle baton de "
                       "contrôle des familiers%s\n", C_GRN, C_WHT, C_GRN, C_WHT);
                printf("%sLe guerrier : %sVenu du Nord, ce puissant guerrier entendu parler de l’épée flamboyante, "
                       "une arme tellement puissante capable d’enflammer tout ce qui se trouve sur son passage. En quête "
                       "de gloire et de pouvoir, l’avidité du guerrier l’emmènera jusqu’au bout de ce profond donjon pour "
                       "en ressortir vainqueur.\n", C_ORE, C_WHT);
                printf("%sLe voleur : %sLe voleur, un homme vicieux prêt à tout pour l’argent. Envieux des plus "
                       "riches il décide de s’emparer de la dague de sommeil se trouvant dans le donjon afin d’élaborer "
                       "des plans sournois pour s’enrichir sans se faire prendre. Cependant arrivera-t-il à ses fins ? \n",
                       C_LGR, C_WHT);
                printf("%sLe magicien : %sLe magicien n’a qu’une quête, récupérer le grimoire interdit se trouvant "
                       "dans ce donjon. Son unique but est de pouvoir ramener à la vie l’élu de son coeur qu’il a perdu "
                       "lors d’une guerre. Peu importe les obstacles qu’il rencontrera, il ne baissera pas les bras même "
                       "si cela pourrait lui coûter la vie...\n", C_LBE, C_WHT);
                waiting();
                break;
            case '3':
                printf("%sLe systeme de combats : %s\n", C_BLU, C_WHT);
                printf("Le systeme est simple. On choisit une arme, et si l'arme correspond au monstre, alors "
                       "le combat est gagné. Sinon, c'est perdu. On peut également essayer de se battre à mains nues "
                       "mais ce n'est pas vraiment conseillé..\n");
                waiting();
                break;
            case '4':
                printf("%sLes monstres :%s\n", C_PUE, C_WHT);
                printf("Ils sont au nombre de 4 espèces : le Basilic %s; le Zombie %s; le Troll %s; la Harpie %s\n",
                       BASILISK, ZOMBIE, TROLL, HARPY);
                printf("\nLes Basilics, serpents géants à la morsure mortelle et au regard pétrifiant. On dit que "
                       "seule une surface réfléchissante permet de le vaincre...\n");
                printf("\nLe zombie, imperturbable face à toute arme conventionnelle, succombe uniquement devant "
                       "le pouvoir flamboyant...\n");
                printf("\nLes Trolls, des adversaires colossaux, ne fléchiront qu'en face de la fureur déchaînée des "
                       "haches à deux mains, portant en elles le pouvoir destructeur absolu.\n");
                printf("\nLes Harpies, des créatures volantes monstrueuses, se trouvent vulnérables face aux armes de jet. "
                       "Un arc s'avère être la solution idéale pour les affronter avec succès.\n");
                waiting();
                break;
            case '5':
                object_menu();
                break;
            case '6':
                return; //on quitte le menu pour revenir dans title screen
            default:
                //rien par défaut
                break;
        }
        tuto_menu_text(); //le texte récurrent
    }
}

//le printf récurrent de title screen, utilisé dans une boucle
void title_screen_text(){
    clear_all();
    printf("%s%sBienvenue dans %sMyster'us Adventures\n", B_BLK, C_WHT, C_RED);
    printf("\n\n\n%s[1] JOUER\n[2] SCORES\n[3] TUTORIEL\n", C_WHT);
}

void title_screen() {
    title_screen_text(); //le texte du title screen, il est aussi print dans la boucle
    int choice;
    while ((choice = getchar()) != EOF) {
        discardInput(); //vide le buffer
        switch (choice) {
            case '1': // touche 1 pressée
                launch_game(); //lance le jeu
                break;
            case '2': // touche 2 pressée
                open_scores(); //ouvre le menu de score
                break;
            case '3': // touche 3 pressée
                tuto_menu(); //ouvre le menu tutoriel
                break;
            case '8':
                printf("%sMerci Jeuxjeux20\n", C_LBE);
                waiting();
            default:
                //aucun comportement prévu pour cette touche
                break;
        }
        title_screen_text();
    }
}
