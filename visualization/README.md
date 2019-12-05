- Prerequisite:

`node.js` [Download here](https://nodejs.org/en/download/)

`node http-server` [Download here](https://www.npmjs.com/package/http-server)

- How to run:

1. make sure you are in `visualization` folder
2. run `http-server . -p 8000`
3. access `localhost:8000` in a browser that allows CORS (see Notes)

- Notes:

1. How to allow CORS in my browser?

Firefox

    Go to about:config (type the address in the Firefox navigation bar)
    Find security.fileuri.strict_origin_policy parameter
    Set it to false

Chrome

    Close all running Chrome instances first. The important word here is ‘all’. On Windows, you may check for Chrome instances using the Windows Task Manager. Alternatively, if you see a Chrome icon in the system tray, then you may open its context menu and click ‘Exit’. This should close all Chrome instances.
    Start a command prompt / terminal. On Windows, type Command Prompt in the Search box, and then, in the list of results, double-click Command Prompt.
    Find the Chrome executable folder. On Windows, search for the file chrome.exe in a Navigator windows.
    Then start the Chrome executable with a command line flag: path_to_chrome/chrome --allow-file-access-from-files

Internet Explorer

    Theoretically, Internet Explorer allows changes in the local files security policy. However, in our tests the options had no effect under Windows 8.0.

Safari

    Enable the develop menu using the preferences panel, under Advanced → “Show develop menu in menu bar”. Then from the safari “Develop” menu, select “Disable local file restrictions”.

    It is also worth noting Safari has some odd behavior with caches, so it is advisable to use the “Disable caches” option in the same menu if you are editing & debugging using safari.