Notion Post:
[https://ms-jahan.notion.site/Wake-Up-PC-WakeOnLan-Remotely-Using-Telegram-and-NodeMCU-500802b5219949b1865ba849cbc390dc](https://ms-jahan.notion.site/Wake-Up-PC-WakeOnLan-Remotely-Using-Telegram-and-NodeMCU-500802b5219949b1865ba849cbc390dc)

## Before Getting Started

Please change the Wi-Fi name (SSID), Wi-Fi password, Bot Token, Chat ID, and Mac Address before using it.
## Step By Step Procedure

### NodeMCU Side

1. Install NodeMCU driver. Mine was used from [here](https://www.arduined.eu/ch340-windows-10-driver-download/).
2. Install [Arduino IDE](https://www.arduino.cc/en/software).
3. Open Arduino IDE.
4. Click File > Preference.
5. Add This URL "http://arduino.esp8266.com/stable/package_esp8266com_index.json" to Additional Board Manager URLs.
6. Go to Tools > Board > Board Manager.
7. Wait until the update is complete.
8. Search esp8266 by ESP8266 Comunity, then install, and wait until the installation is complete.
9. After it finishes, Go to Tools > Board > NodeMCU 1.0 (ESP-12E Module). Mine worked for `Generic 8266`. More Info [here](https://www.instructables.com/Get-Started-With-ESP8266-NodeMCU-Lolin-V3/).
10. Connect NodeMCU to the PC using the required USB cable and select the port.
11. Install `UniversalTelegramBot` and `WakeOnLan` from the libraries section.
12. Click Verify (tick icon) to check if everything is OK.
13. Click Upload to flash the NodeMCU.

### PC Side

To enable WoL on Windows 10 (should work on 11 also), use these steps (from [windowscentral](https://www.windowscentral.com/how-enable-and-use-wake-lan-wol-windows-10)):

1. Open **Settings**.
2. Click on **Network & Internet**.
3. Click on **Status**.
4. Under the "Advanced network settings" section, click the **Change adapter options** button.
5. Right-click the active network adapter and select the **Properties** option.
6. Click the **Networking** tab.
7. Click the **Configure** button.
8. Click the **Advanced** tab.
9. Select the **Wake on Magic Packet** option.
10. Use the "Value" drop-down menu and select the **Enabled** option.
11. (Optional) Select the **Wake on pattern match** option.
12. Use the "Value" drop-down menu and select the **Enabled** option.
13. (Optional) Select the **WoL & Shutdown Link Speed** option.
14. Use the "Value" drop-down menu and select the **10Mbps** option.
- **Quick note:** The optional settings may be required if you use a network adapter from Realtek.
15. Click the **Power Management** tab.
16. Check the **Allow this device to wake the computer** option.
17. Check the **Only allow a magic packet to wake the computer** option. Only enable this if you want to use the magic packet. Otherwise, don’t tick it.
18. Click the **OK** button.

**Now boot into BIOS and enable Wake On Lan and relevant settings.**

Also, disable Fast Boot on Windows. [# Source](https://learn.microsoft.com/en-us/troubleshoot/windows-client/deployment/wake-on-lan-feature)

1. In the **Control Panel**, open the **Power Options** item.
2. Select the **Choose what the power buttons do** link.
3. Clear the **Turn on fast startup (recommended)** check box.
4. Select **Save Settings**.

After the PC wakes up and boots into OS, you can control it using TeamViewer (unattended Mode), Google Remote Desktop, or relevant remote desktop software.
