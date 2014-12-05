<?php
/*
Copyright (©) 2003-2014 Teus Benschop.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
?>
<h2><?php echo gettext("Install Bibledit-Web 1.9 (not yet released) on PCLinuxOS LXDE 2014") ?></h2>
<p>
  The pre-release of version 1.9 is available here:
  http://bibleconsultants.nl/downloads/temporal/bibledit-web-1.8.32.zip.
</p>
<p>
  <?php echo gettext("Follow the steps below to install Bibledit-Web.") ?>
</p>
<ol>
<li><?php echo gettext("Open the Synaptic Package Manager.") ?></li>
<li>
  <p><?php echo gettext("Install the following packages:") ?></p>
  <p><code>task-lamp</code></p>
  <p><code>php-iconv</code></p>
</li>
<li>
  <?php echo gettext("To activate the web server, open Configure your Computer, click Sharing, click Configure web server.") ?>
  <?php echo gettext("Follow the steps in the wizard, taking the default settings, and clicking Next, till it finishes.") ?>
</li>
<li>
  <?php echo gettext("Test the web server by opening the site in the browser:") ?>
  http://localhost.
  <?php echo gettext("The browser should say that the server is alive and working.") ?>
</li>
<li>
  <?php echo gettext("Open a Terminal.") ?>
</li>
<li>
  <?php echo gettext("Become the administrative user:") ?>
  <p><code>$ su</code></p>
  <p><?php echo gettext("Provide the administrative password.") ?></p>
</li>
<li>
  <p><?php echo gettext("Download and install Bibledit-Web in folder /var/www/html:") ?></p>
  <p><code>$ cd /var/www/html</code></p>
  <p><code>$ wget http://download.savannah.nongnu.org/releases/bibledit-web/source/bibledit-web-1.9.zip</code></p>
  <p><code>$ unzip bibledit-web-1.9.zip</code></p>
  <p><code>$ chown -R apache:apache bibledit-web</code></p>
</li>
<li><?php echo gettext("Bibledit-Web will be accessible through:") ?> http://localhost/bibledit-web.</li>
<li><?php echo gettext("Open the web address in the browser, and follow the steps on the screen to configure Bibledit-Web and log in.") ?></li>
<li><?php echo gettext("Reboot.") ?></li>
</ol>
