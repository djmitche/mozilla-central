/* Any copyright is dedicated to the Public Domain.
* http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

function runPaneTest(fn) {
  function observer(win, topic, data) {
    Services.obs.removeObserver(observer, "advanced-pane-loaded");

    let policy = Components.classes["@mozilla.org/datareporting/service;1"]
                                   .getService(Components.interfaces.nsISupports)
                                   .wrappedJSObject
                                   .policy;
    ok(policy, "Policy object defined");

    fn(win, policy);
  }

  Services.obs.addObserver(observer, "advanced-pane-loaded", false);
  openDialog("chrome://browser/content/preferences/preferences.xul", "Preferences",
             "chrome,titlebar,toolbar,centerscreen,dialog=no", "paneAdvanced");
}

function test() {
  waitForExplicitFinish();
  resetPreferences();
  registerCleanupFunction(resetPreferences);

  Services.prefs.lockPref("datareporting.healthreport.uploadEnabled");
  runPaneTest(testUploadDisabled);
}

function testUploadDisabled(win, policy) {
  ok(policy.healthReportUploadLocked, "Upload enabled flag is locked.");
  let checkbox = win.document.getElementById("submitHealthReportBox");
  is(checkbox.getAttribute("disabled"), "true", "Checkbox is disabled if upload setting is locked.");
  policy._healthReportPrefs.unlock("uploadEnabled");

  win.close();
  runPaneTest(testBasic);
}

function testBasic(win, policy) {
  let doc = win.document;

  is(policy.dataSubmissionPolicyAccepted, false, "Data submission policy not accepted.");
  is(policy.healthReportUploadEnabled, true, "Health Report upload enabled on app first run.");

  let checkbox = doc.getElementById("submitHealthReportBox");
  ok(checkbox);
  is(checkbox.checked, true, "Health Report checkbox is checked on app first run.");

  checkbox.checked = false;
  checkbox.doCommand();
  is(policy.healthReportUploadEnabled, false, "Unchecking checkbox opts out of FHR upload.");

  checkbox.checked = true;
  checkbox.doCommand();
  is(policy.healthReportUploadEnabled, true, "Checking checkbox allows FHR upload.");

  win.close();
  finish();
}

function resetPreferences() {
  Services.prefs.clearUserPref("datareporting.healthreport.uploadEnabled");
}

