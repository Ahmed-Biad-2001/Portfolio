using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Morpion
{
    // Ceci est la classe qui gère toute la partie logique du jeu
    class Jeu
    {
        private IdJoueur[] grille = new IdJoueur[9];
        public IdJoueur[] Grille { get { return grille; } }
        private IdJoueur tour;
        public IdJoueur Tour { get { return tour; } }
        private IdJoueur gagnant ;
        public IdJoueur Gagnant { get { return gagnant; } }

        public int scoreS=0, scoreC=0; // Score du Serveur et du Client

        public Jeu() { InitialiserJeu(); }

        // On vide tous les cases, et on choisit un joueur aléatoirement qui commence
        public void InitialiserJeu()
        {
            for (int i = 0; i < 9; i++) grille[i] = IdJoueur.Personne;
            gagnant = IdJoueur.Personne;

            Random rand = new Random();
            if (rand.Next(2) == 1) tour = IdJoueur.Serveur;
            else tour = IdJoueur.Client;
        }

        // Retourne 0 si les arguments sont incorrects (case déjà pleine ou bien jeu terminé)
        // Retourne 1 si c'est correct
        // Retourne 2 si c'est corretct et la partie vient de se terminer
        public int JouerCase(int c)
        {
            if (c < 0 || c > 8)
                return 0;

            if (grille[c] != IdJoueur.Personne ||  gagnant != IdJoueur.Personne)
            {
                return 0;
            }
            else
            {
                grille[c] = tour;
                if (tour == IdJoueur.Serveur) tour = IdJoueur.Client;
                else tour = IdJoueur.Serveur;

                if (EvaluerJeu()) return 2;
                else return 1;
            }
        }

        // Retourne vrai si la partie est finie
        public bool EvaluerJeu()
        {
            //      0 | 1 | 2
            //      ----------
            //      3 | 4 | 5
            //      ----------
            //      6 | 7 | 8
            if (grille[0] == grille[1] && grille[0] == grille[2] && grille[0] != IdJoueur.Personne)
                gagnant = grille[0];
            if (grille[3] == grille[4] && grille[3] == grille[5] && grille[3] != IdJoueur.Personne)
                gagnant = grille[3];
            if (grille[6] == grille[7] && grille[6] == grille[8] && grille[6] != IdJoueur.Personne)
                gagnant = grille[6];
            if (grille[0] == grille[3] && grille[0] == grille[6] && grille[0] != IdJoueur.Personne)
                gagnant = grille[0];
            if (grille[1] == grille[4] && grille[1] == grille[7] && grille[1] != IdJoueur.Personne)
                gagnant = grille[1];
            if (grille[2] == grille[5] && grille[2] == grille[8] && grille[2] != IdJoueur.Personne)
                gagnant = grille[2];
            if (grille[0] == grille[4] && grille[0] == grille[8] && grille[0] != IdJoueur.Personne)
                gagnant = grille[0];
            if (grille[2] == grille[4] && grille[2] == grille[6] && grille[2] != IdJoueur.Personne)
                gagnant = grille[2];


            if (grille.Count(c => c == IdJoueur.Personne) == 0  // On compte le nombre de cases vide, et vérifie si ça vaut 0
                || gagnant != IdJoueur.Personne)                // On vérifie si il y a un gagnant déterminé
            {
                // On incrémente le score du gagant
                if (gagnant == IdJoueur.Client) scoreC++;       
                else if (gagnant == IdJoueur.Serveur) scoreS++;
                return true;
            }
            else
                return false;
        }
    }
}
