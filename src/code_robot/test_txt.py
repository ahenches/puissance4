from pyniryo import *

robot = NiryoRobot("10.10.101.38")
robot.calibrate_auto()
robot.play_sound('Presentation.wav', wait_end=True)
robot.move_joints(-0.064, 0.242, -0.1331, -0.199, 0.509, -0.041) #il se lève
robot.move_joints(-0.041, 0.302, -0.349, -0.132, 0.153, 0.066) # se redresse
robot.move_joints(-0.041, 0.345, -0.42, -0.146, -0.77, 0.066) # hoche pince bas
robot.move_joints(-0.041, 0.345, -0.437, -0.15, 0.195, 0.066) #hoche la pince milieux
robot.play_sound('ok,iplay.wav', wait_end=True)
# Fonction 1
def fonction_un():

    print("Fonction 1 exécutée")

# Fonction 2
def fonction_deux():
    print("Fonction 2 exécutée")

# Fonction 3
def fonction_trois():
    print("Fonction 3 exécutée")

# Fonction 4
def fonction_quatre():
    print("Fonction 4 exécutée")

# Fonction 5
def fonction_cinq():
    print("Fonction 5 exécutée")

# Boucle principale
with open("codepuissance4.txt", "r") as fichier:
    choix = fichier.readline().strip()

    # Boucle principale
    while True:
        # Si le choix n'est pas compris entre 1 et 5, redemande à l'utilisateur de saisir un chiffre
        if choix not in ["1", "2", "3", "4", "5"]:
            choix = input("Veuillez saisir un chiffre entre 1 et 5 : ")
        else:
            break

    # Exécute la fonction correspondant au choix de l'utilisateur

if choix == "1":
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.move_joints(-0.89, -0.307, -0.828, -1.236, 0.135, -0.133)  # MOVE AVANT CBOULE
        robot.move_joints(-0.688, -0.46, -0.517, -0.405, -0.612, -0.291)  # MOVE AVANT CBOULE
        robot.move_joints(-0.805, -0.655, -0.455, -0.215, -0.496, -0.112)  # va chercher la boule
        robot.pull_air_vacuum_pump()  # aspire la boule
        robot.move_joints(-0.89, -0.307, -0.828, -1.236, 0.135, -0.133)  # MOVE AVANT CBOULE
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.move_joints(-0.256, -0.378, 0.183, -0.196, -0.376, 0.318)  # MOVE APRES CBOULE COLONNE 1
        robot.push_air_vacuum_pump()  # relache la boule
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.play_sound('itsurturn.wav', wait_end=True)
        #robot.close_connection()
elif choix == "2":
        robot.move_joints(-0.89, -0.307, -0.828, -1.236, 0.135, -0.133)  # MOVE AVANT CBOULE
        robot.move_joints(-0.688, -0.46, -0.517, -0.405, -0.612, -0.291)  # MOVE AVANT CBOULE
        robot.move_joints(-0.805, -0.655, -0.455, -0.215, -0.496, -0.112)  # va chercher la boule
        robot.pull_air_vacuum_pump()  # aspire la boule
        robot.move_joints(-0.89, -0.307, -0.828, -1.236, 0.135, -0.133)  # MOVE AVANT CBOULE
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.move_joints(-0.131, -0.328, 0.011, -0.139, -0.12, 0.017)  # MOVE APRES CBOULE COLONNE 2
        robot.push_air_vacuum_pump()  # relache la boule
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.play_sound('itsurturn.wav', wait_end=True)
        #robot.close_connection()
elif choix == "3":
        robot.move_joints(-0.89, -0.307, -0.828, -1.236, 0.135, -0.133)  # MOVE AVANT CBOULE
        robot.move_joints(-0.688, -0.46, -0.517, -0.405, -0.612, -0.291)  # MOVE AVANT CBOULE
        robot.move_joints(-0.805, -0.655, -0.455, -0.215, -0.496, -0.112)  # va chercher la boule
        robot.pull_air_vacuum_pump()  # aspire la boule
        robot.move_joints(-0.89, -0.307, -0.828, -1.236, 0.135, -0.133)  # MOVE AVANT CBOULE
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.move_joints(0.015, -0.278, -0.055, -0.026, -0.097, -0.041)  # MOVE APRES CBOULE COLONNE 3
        robot.push_air_vacuum_pump()  # relache la boule
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.play_sound('itsurturn.wav', wait_end=True)
        #robot.close_connection()
elif choix == "4":
        robot.move_joints(-0.89, -0.307, -0.828, -1.236, 0.135, -0.133)  # MOVE AVANT CBOULE
        robot.move_joints(-0.688, -0.46, -0.517, -0.405, -0.612, -0.291)  # MOVE AVANT CBOULE
        robot.move_joints(-0.805, -0.655, -0.455, -0.215, -0.496, -0.112)  # va chercher la boule
        robot.pull_air_vacuum_pump()  # aspire la boule
        robot.move_joints(-0.89, -0.307, -0.828, -1.236, 0.135, -0.133)  # MOVE AVANT CBOULE
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.move_joints(0.151, -0.335, 0.007, -0.005, -0.181, 0.025)  # MOVE APRES CBOULE COLONNE 4
        robot.push_air_vacuum_pump()  # relache la boule
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.play_sound('itsurturn.wav', wait_end=True)
        #robot.close_connection()
elif choix == "5":
        robot.move_joints(-0.89, -0.307, -0.828, -1.236, 0.135, -0.133)  # MOVE AVANT CBOULE
        robot.move_joints(-0.688, -0.46, -0.517, -0.405, -0.612, -0.291)  # MOVE AVANT CBOULE
        robot.move_joints(-0.805, -0.655, -0.455, -0.215, -0.496, -0.112)  # va chercher la boule
        robot.pull_air_vacuum_pump()  # aspire la boule
        robot.move_joints(-0.89, -0.307, -0.828, -1.236, 0.135, -0.133)  # MOVE AVANT CBOULE
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.move_joints(0.273, -0.369, 0.093, -0.093, -0.232, 0.008)  # MOVE APRES CBOULE COLONNE 5
        robot.push_air_vacuum_pump()  # relache la boule
        robot.move_joints(0.175, 0.259, -1.316, 0.052, 0.531, -0.135)  # revient position initial
        robot.play_sound('itsurturn.wav', wait_end=True)
        #robot.close_connection()
else:
        print("Choix invalide. Veuillez saisir un chiffre entre 1 et 5.")
