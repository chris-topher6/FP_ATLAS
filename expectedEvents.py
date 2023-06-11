import json

# from infofile.txt
bg_processes = {
                    "ttbar": {
                        "Events": 7847944,
                        "xsec[pb]": 252.82
                        },
                    "singletop": {
                        "Events": 1468942,
                        "xsec[pb]": 52.47
                        },
                    "diboson": {
                        "Events": 922521,
                        "xsec[pb]": 29.41
                        },
                    "zjets": {
                        "Events": 37422926,
                        "xsec[pb]": 2516.2
                        },
                    "wjets": {
                        "Events": 66536222,
                        "xsec[pb]": 36214
                        },
                    "zprime400": {
                        "Events": 100000,
                        "xsec[pb]": 110
                        },
                    "zprime500": {
                        "Events": 100000,
                        "xsec[pb]": 82
                        },
                    "zprime750": {
                        "Events": 100000,
                        "xsec[pb]": 20
                        },
                    "zprime1000": {
                        "Events": 100000,
                        "xsec[pb]": 5.5
                        },
                    "zprime1250": {
                        "Events": 100000,
                        "xsec[pb]": 1.9
                        },
                    "zprime1500": {
                        "Events": 100000,
                        "xsec[pb]": 0.83
                        },
                    "zprime1750": {
                        "Events": 100000,
                        "xsec[pb]": 0.3
                        },
                    "zprime2000": {
                        "Events": 100000,
                        "xsec[pb]": 0.14
                        },
                    "zprime2250": {
                        "Events": 100000,
                        "xsec[pb]": 0.067
                        },
                    "zprime2500": {
                        "Events": 100000,
                        "xsec[pb]": 0.035
                        },
                    "zprime3000": {
                        "Events": 100000,
                        "xsec[pb]": 0.012
                        }
                }

# create a list of filepaths to the efficiency files
filenames = []
file_extensions =[".el_efficiency.txt", ".mu_efficiency.txt"]
for extension in file_extensions:
    for process in bg_processes:
        filename = "output_runSelection/" + process + extension
        filenames.extend([filename])

#print(filenames) # works as planned!

# read the efficiencies for data.el and data.mu into the bg_processes dict

for file_path in filenames:

    with open(file_path, 'r') as file:
        process_name = file_path[20:-15]

        # only look at the second line and extract the value of the efficiency
        lines = file.readlines()
        line = lines[1].strip()
        key, value = line.split(": ")

        '''
        # Debugging
        if key == "total Efficiency":
            try:
                x = float(value)
            except ValueError:
                print("Error: The line does not contain a valid number.")
                x = None
            else:
                print("The extracted number from ", process_name, "is:", x)
        '''

        # add the mu and el efficiencies respectively to the dictionary
        bg_processes[process_name[:-3]][process_name[-2:] + "_eff"] = float(value)


# print(bg_processes) # works as planned!

# calculate the number of expected events N for each process (background processes and zprime hypotheses)

L = float(1000) # pb^-1

for process in bg_processes:

    sigma = float(bg_processes[process]["xsec[pb]"])

    # expected events in the mu data set
    A_x_eps = bg_processes[process]["mu_eff"]
    bg_processes[process]["N_mu"] = L * sigma * (A_x_eps)   

    # expected events in the el data set
    A_x_eps = bg_processes[process]["el_eff"]
    bg_processes[process]["N_el"] = L * sigma * (A_x_eps)

    # scalefactor
    bg_processes[process]["scalefactor"] = L * sigma / (bg_processes[process]["Events"])


# save the dictionary to a .txt file
json_data = json.dumps(bg_processes, indent=4)

with open("6_1.txt", 'w') as file:

    file.write(json_data)

    print("saved to 6_1.txt")

# saving the scale factors

zprime_masses = [   "400",
                    "500",
                    "750",
                    "1000",
                    "1250",
                    "1500",
                    "1750",
                    "2000",
                    "2250",
                    "2500",
                    "3000"]

with open("scale_test.txt", 'w') as file:

    file.write(str(bg_processes['diboson']['scalefactor']))
    file.write("\n")
    file.write(str(bg_processes['singletop']['scalefactor']))
    file.write("\n")
    file.write(str(bg_processes['ttbar']['scalefactor']))
    file.write("\n")
    file.write(str(bg_processes['wjets']['scalefactor']))
    file.write("\n")
    file.write(str(bg_processes['zjets']['scalefactor']))

    for mass in zprime_masses:
        file.write("\n")
        file.write(str(bg_processes["zprime" + mass]['scalefactor']))
        

    print("saved to scale_test.txt")