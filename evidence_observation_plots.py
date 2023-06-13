# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

# Konfiguration der Schriftart
plt.rcParams['font.family'] = 'DejaVu Sans'


#Plots für Evidence

xsec = [110.0, 82.0, 20.0, 5.5, 1.9, 0.83, 0.3, 0.14, 0.067, 0.035, 0.012]
masses = [400, 500, 750, 1000, 1250, 1500, 1750, 2000, 2250, 2500, 3000]
xsec_3jet_el = [110.0, 20.0, 5.5, 1.9, 0.83, 0.3, 0.14, 0.067, 0.035, 0.012]
xsec_3jet_mu = [110.0, 82.0, 20.0, 5.5, 1.9, 0.83, 0.3, 0.14, 0.067, 0.035, 0.012]
xsec_sys_el = [110.0, 82.0, 20.0, 5.5, 1.9, 0.83, 0.3, 0.14, 0.067, 0.035, 0.012]
xsec_sys_mu = [5.5, 1.9, 0.83, 0.3, 0.14, 0.067, 0.035, 0.012]

# Daten aus der Textdatei lesen
data_3jet_el = np.loadtxt("limits/Evidence_inv_mass_3jet_el.txt", skiprows=2, delimiter='\t', dtype={'names': ('p_value', 'mass', 'scale'), 'formats': ('f8', 'i4', 'i4')})
data_3jet_mu = np.loadtxt("limits/Evidence_inv_mass_3jet_mu.txt", skiprows=2, delimiter='\t', dtype={'names': ('p_value', 'mass', 'scale'), 'formats': ('f8', 'i4', 'i4')})
data_sys_el = np.loadtxt("limits/Evidence_inv_mass_sys_el.txt", skiprows=2, delimiter='\t', dtype={'names': ('p_value', 'mass', 'scale'), 'formats': ('f8', 'i4', 'i4')})
data_sys_mu = np.loadtxt("limits/Evidence_inv_mass_sys_mu.txt", skiprows=2, delimiter='\t', dtype={'names': ('p_value', 'mass', 'scale'), 'formats': ('f8', 'i4', 'i4')})


# Extrahiere die einzelnen Spalten

masses_3jet_el = data_3jet_el['mass']
scale_3jet_el = data_3jet_el['scale']
xsec_3jet_el = scale_3jet_el * xsec_3jet_el

masses_3jet_mu = data_3jet_mu['mass']
scale_3jet_mu = data_3jet_mu['scale']
xsec_3jet_mu = scale_3jet_mu * xsec_3jet_mu

masses_sys_el = data_sys_el['mass']
scale_sys_el = data_sys_el['scale']
xsec_sys_el = scale_sys_el * xsec_sys_el

masses_sys_mu = data_sys_mu['mass']
scale_sys_mu = data_sys_mu['scale']
xsec_sys_mu = scale_sys_mu * xsec_sys_mu

# Plot erstellen
plt.plot(masses, xsec, linestyle='--', label='unscaled')
plt.plot(masses_3jet_el, xsec_3jet_el, linestyle='-', label='3jet el')
plt.plot(masses_3jet_mu, xsec_3jet_mu, linestyle='-', label='3jet mu')
plt.plot(masses_sys_el, xsec_sys_el, linestyle='-', label='sys el')
plt.plot(masses_sys_mu, xsec_sys_mu, linestyle='-', label='sys mu')

# Achsentitel und Legende hinzufügen
plt.xlabel('Mass')
plt.ylabel('xsec')
plt.legend()
plt.savefig('evidence_observation/evidence.pdf')


# Plot für Observation

xsec_3jet_el = [110.0, 82.0, 20.0, 5.5, 1.9, 0.83, 0.3, 0.14, 0.067, 0.035, 0.012]
xsec_3jet_mu = [110.0, 82.0, 20.0, 5.5, 1.9, 0.83, 0.3, 0.14, 0.067, 0.035, 0.012]
xsec_sys_el  = [110.0, 82.0, 20.0, 5.5, 1.9, 0.83, 0.3, 0.14, 0.067, 0.035, 0.012]
xsec_sys_mu  = [110.0, 82.0, 20.0, 5.5, 1.9, 0.83, 0.3, 0.14, 0.067, 0.035, 0.012]

# Daten aus der Textdatei lesen
data_3jet_el = np.loadtxt("limits/Observation_inv_mass_3jet_el.txt", skiprows=2, delimiter='\t', dtype={'names': ('p_value', 'mass', 'scale'), 'formats': ('f8', 'i4', 'i4')})
data_3jet_mu = np.loadtxt("limits/Observation_inv_mass_3jet_mu.txt", skiprows=2, delimiter='\t', dtype={'names': ('p_value', 'mass', 'scale'), 'formats': ('f8', 'i4', 'i4')})
data_sys_el = np.loadtxt("limits/Observation_inv_mass_sys_el.txt", skiprows=2, delimiter='\t', dtype={'names': ('p_value', 'mass', 'scale'), 'formats': ('f8', 'i4', 'i4')})
data_sys_mu = np.loadtxt("limits/Observation_inv_mass_sys_mu.txt", skiprows=2, delimiter='\t', dtype={'names': ('p_value', 'mass', 'scale'), 'formats': ('f8', 'i4', 'i4')})


# Extrahiere die einzelnen Spalten

masses_3jet_el = data_3jet_el['mass']
scale_3jet_el = data_3jet_el['scale']
xsec_3jet_el = scale_3jet_el * xsec_3jet_el

masses_3jet_mu = data_3jet_mu['mass']
scale_3jet_mu = data_3jet_mu['scale']
xsec_3jet_mu = scale_3jet_mu * xsec_3jet_mu

masses_sys_el = data_sys_el['mass']
scale_sys_el = data_sys_el['scale']
xsec_sys_el = scale_sys_el * xsec_sys_el

masses_sys_mu = data_sys_mu['mass']
scale_sys_mu = data_sys_mu['scale']
xsec_sys_mu = scale_sys_mu * xsec_sys_mu

# Plot erstellen
plt.figure()
plt.plot(masses, xsec, linestyle='--', label='unscaled')
plt.plot(masses_3jet_el, xsec_3jet_el, linestyle='-', label='3jet el')
plt.plot(masses_3jet_mu, xsec_3jet_mu, linestyle='-', label='3jet mu')
plt.plot(masses_sys_el, xsec_sys_el, linestyle='-', label='sys el')
plt.plot(masses_sys_mu, xsec_sys_mu, linestyle='-', label='sys mu')

# Achsentitel und Legende hinzufügen
plt.xlabel('Mass')
plt.ylabel('xsec')
plt.legend()

# Plot anzeigen
plt.savefig('evidence_observation/observation.pdf')