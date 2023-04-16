#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""
Author : ELFRANI Khadija
Project : Robot Niryo  - puissance 4
Filière : IATIC4 - 22/23
Partie : Reconnaissance vocale et application mobile - interfaces de jeu
"""
import speech_recognition as sr
import time
from kivy.clock import Clock
from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.gridlayout import GridLayout
from kivy.uix.button import Button
from kivy.graphics import Color, Rectangle

"""     FENÊTRE JEU AVEC RECO VOCALE    """

class VoiceRecognitionApp(App):
    """
    Author : ELFRANI Khadija
    Function : build()
    Objectif : crée une fenêtre graphique qui contient une grille de boutons pour
    jouer à puissance 4 et un label pour afficher les instructions.
    Retourne la fenêtre graphique.
    """
    def build(self):
        self.title = 'Puissance 4'
        layout = BoxLayout(orientation='vertical')
        layout.canvas.before.add(Color(0, 0, 1, 1))
        layout.canvas.before.add(Rectangle(pos=(0, 0), size=(900, 330)))
        self.label = Label(text='Parlez maintenant...')
        layout.add_widget(self.label)

        self.grid_layout = GridLayout(cols=5, rows=5, size_hint=(1, 0.9))
        self.grid_layout.spacing = [10, 10]

        for i in range(25):
            self.grid_layout.add_widget(
                Button(text='', font_size=40, background_color=(0, 0, 0, 1)))
        layout.add_widget(self.grid_layout)

        self.col_labels = BoxLayout(orientation='horizontal', size_hint=(1, 0.1))
        for i in range(1, 6):
            self.col_labels.add_widget(Label(text=str(i), font_size=30, size_hint=(1 / 5, 1)))
        layout.add_widget(self.col_labels)

        return layout
    """
    Author : ELFRANI Khadija
    Function : on_start()
    Objectif : Initialise la reconnaissance vocale et démarre la reconnaissance.
    """
    def on_start(self):
        self.recognizer = sr.Recognizer()
        self.microphone = sr.Microphone()

        with self.microphone as source:
            self.recognizer.adjust_for_ambient_noise(source)
        self.stop_listening = self.recognizer.listen_in_background(self.microphone, self.callback)

    """
    Author : ELFRANI Khadija
    Function : on_stop()
    Objectif : Arrête la reconnaissance vocale lorsque l'application se ferme.
    """
    def on_stop(self):
        self.stop_listening()

    """
      Author : ELFRANI Khadija
      Function : callback()
      Objectif : Traite la commande vocale reconnue par l'API.
      Params :
            recognizer (speech_recognition.Recognizer): L'objet de reconnaissance vocale.
            audio (speech_recognition.AudioData): Les données audio de l
      """
    def callback(self, recognizer, audio):
        try:
            ordre = recognizer.recognize_google(audio, language='fr-FR')
            if ordre == "colonne 1" or ordre == "première colonne" or ordre == "1e colonne":
                self.allumer_case(4, ordre)
                write_commande_to_file(1)
                time.sleep(2)
                self.allumer_case_joueur2()
                time.sleep(2)
                self.label.text = "Parlez maintenant..."
            elif ordre == "colonne 2" or ordre == "deuxième colonne" or ordre == "2e colonne":
                self.allumer_case(3, ordre)
                write_commande_to_file(2)
                time.sleep(2)
                self.allumer_case_joueur2()
                time.sleep(2)
                self.label.text = "Parlez maintenant..."
            elif ordre == "colonne 3" or ordre == "troisième colonne" or ordre == "3e colonne":
                self.allumer_case(2, ordre)
                write_commande_to_file(3)
                time.sleep(2)
                self.allumer_case_joueur2()
                time.sleep(2)
                self.label.text = "Parlez maintenant..."
            elif ordre == "colonne 4" or ordre == "quatrième colonne" or ordre == "4e colonne":
                self.allumer_case(1, ordre)
                write_commande_to_file(4)
                time.sleep(2)
                self.allumer_case_joueur2()
                time.sleep(2)
                self.label.text = "Parlez maintenant..."
            elif ordre == "colonne 5" or ordre == "cinquième colonne" or ordre == "5e colonne":
                self.allumer_case(0, ordre)
                write_commande_to_file(5)
                time.sleep(2)
                self.allumer_case_joueur2()
                time.sleep(2)
                self.label.text = "Parlez maintenant..."
            elif ordre == "quitter":
                self.stop_listening()
                self.label.text = "Au revoir !"
                App.get_running_app().stop()
            else:
                self.label.text = "Je n'ai pas compris, veuillez réessayer..."
        except sr.UnknownValueError:
            self.label.text = "Je n'ai pas compris, veuillez réessayer..."
        except sr.RequestError as e:
            self.label.text = "Impossible de contacter le service de reconnaissance vocale ({0})".format(e)

    """
      Author : ELFRANI Khadija
      Function : allumer_case()
      Objectif : allume le bouton le plus bas de la colonne donnée en arg à partir de la reco vocale
      params : Colonne : entier , ordre : chaine de caractères
      """
    def allumer_case(self, colonne, ordre):
        for i in range(0, 5):
            button = self.grid_layout.children[i * 5 + colonne]
            if button.text == '':
                button.background_color = [70, 250, 0, 0.8]
                button.text = ' '
                self.label.text = "Je mets la balle dans la " + ordre
                return
        self.label.text = "La " + ordre + "est pleine, veuillez en choisir une autre."

    """
      Author : ELFRANI Khadija
      Function : allumer_case_joueur2()
      Objectif : allume le bouton le plus bas de la colonne a partir du fichier texte
      """
    def allumer_case_joueur2(self):
        colonne = get_commande_from_file()
        if colonne == 1:
            ordre = 'colonne ' + str(colonne)
            colonne = 4
        elif colonne == 0:
            ordre = 'colonne ' + str(colonne)
            colonne = 5
        elif colonne == 2:
            ordre = 'colonne ' + str(colonne)
            colonne = 3
        elif colonne == 3:
            ordre = 'colonne ' + str(colonne)
            colonne = 2
        elif colonne == 4:
            ordre = 'colonne ' + str(colonne)
            colonne = 1
        elif colonne == 5:
            ordre = 'colonne ' + str(colonne)
            colonne = 0
        for i in range(0, 5):
            button = self.grid_layout.children[i * 5 + colonne]
            if button.text == '':
                button.background_color = [255, 0, 0, 0.8]
                button.text = ' '
                self.label.text = "Le joueur 2 a choisi " + ordre
                return
        self.label.text = "La " + ordre + " est pleine, veuillez en choisir une autre."
"""
      Author : ELFRANI Khadija
      Function : get_commande_from_file()
      Objectif : ouvre le fichier de commande physique et recupère le contenu
      renvoie le contenu ( entier ) du fichier
      """
def get_commande_from_file():
    with open('commande_physique.txt', 'r') as f:
        commande_physique = int(f.read())
    return commande_physique

"""
      Author : ELFRANI Khadija
      Function : write_commande_to_file()
      Objectif : ouvre le fichier de commande vocale et écrit dedans la nouvelle commande dite
"""
def write_commande_to_file(commande):
    with open("commande_vocale.txt", "w") as fichier:
        fichier.write(str(commande))

"""     FENÊTRE JEU AVEC CLIQUE    """

class ManualModeApp(App):
    """
    Author : ELFRANI Khadija
    Function : build()
    Objectif : crée une fenêtre graphique qui contient une grille de boutons pour
    jouer à puissance 4 et un label pour afficher les instructions.
    Retourne la fenêtre graphique.
    """
    def build(self):
        self.title = 'Puissance 4'
        layout = BoxLayout(orientation='vertical')
        layout.canvas.before.add(Color(0, 0, 1, 1))
        layout.canvas.before.add(Rectangle(pos=(0, 0), size=(900, 330)))
        self.label = Label(text='Choisissez une colonne ...')
        layout.add_widget(self.label)

        self.grid_layout = GridLayout(cols=5, rows=5, size_hint=(1, 0.9))
        self.grid_layout.spacing = [10, 10]

        for i in range(25):
            button = Button(text='', font_size=40, background_color=(0, 0, 0, 1))
            button.bind(on_press=self.on_button_press)  # lier l'événement on_press à la fonction on_button_press
            self.grid_layout.add_widget(button)

        layout.add_widget(self.grid_layout)

        self.col_labels = BoxLayout(orientation='horizontal', size_hint=(1, 0.1))
        for i in range(1, 6):
            self.col_labels.add_widget(Label(text=str(i), font_size=30, size_hint=(1 / 5, 1)))
        layout.add_widget(self.col_labels)

        return layout
    """
    Author : ELFRANI Khadija
    Function : on_button_press()
    Objectif : déclenche un ensemble d'evenement en clique sur une colonne.
    """
    def on_button_press(self, button):
        colonne = button.parent.children.index(button) % 5 + 1
        self.tour_joueur1(colonne)
        self.label.text = 'Au tour de joueur physique, patientez..'
        Clock.schedule_once(self.perform_tour_joueur2, 3)
    """
    Author : ELFRANI Khadija
    Function : perform_tour_joueur2()
    Objectif : attends 3 seconds avant d'exécuter la commande de joueur 2
    """
    def perform_tour_joueur2(self, dt):
        self.tour_joueur2()
        self.label.text = 'A votre tour, choisissez une colonne...'
    """
    Author : ELFRANI Khadija
    Function : tour_joueur1() et tour_joueur2
    Objectif : allume le bouton tous bas de la colonne correspondante.
    """

    def tour_joueur1(self, colonne):
        if colonne == 1:
            colonne = 0
        elif colonne == 2:
            colonne = 1
        elif colonne == 3:
            colonne = 2
        elif colonne == 4:
            colonne = 3
        elif colonne == 5:
            colonne = 4
        for i in range(0, 5):
            button = self.grid_layout.children[i * 5 + colonne]
            if button.text == '':
                button.background_color = [70, 250, 0, 0.8]
                button.text = ' '
                return

    def tour_joueur2(self):
        colonne = get_commande_from_file()
        if colonne == 1:
            ordre = 'colonne ' + str(colonne)
            colonne = 4
        elif colonne == 0:
            ordre = 'colonne ' + str(colonne)
            colonne = 5
        elif colonne == 2:
            ordre = 'colonne ' + str(colonne)
            colonne = 3
        elif colonne == 3:
            ordre = 'colonne ' + str(colonne)
            colonne = 2
        elif colonne == 4:
            ordre = 'colonne ' + str(colonne)
            colonne = 1
        elif colonne == 5:
            ordre = 'colonne ' + str(colonne)
            colonne = 0
        for i in range(0, 5):
            button = self.grid_layout.children[i * 5 + colonne]
            if button.text == '':
                button.background_color = [255, 0, 0, 0.8]
                button.text = ' '
                self.label.text = "Le joueur 2 a choisi " + ordre
                return
        self.label.text = "La " + ordre + " est pleine, veuillez en choisir une autre."

if __name__ == '__main__':
    print("1. Reconnaissance vocale ")
    print("2. Manuelle ")
    mode = int(input("Choisissez le mode : "))
    if mode == 1 :
        VoiceRecognitionApp().run()
    elif mode == 2 :
        ManualModeApp().run()
