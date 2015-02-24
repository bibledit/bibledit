<?php
/*
Copyright (©) 2003-2015 Teus Benschop.

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
<br>
<table>
  <tr>
    <td>translate("Old"):</td>
    <td>$this->old_text</td>
  </tr>
  <tr>
    <td>translate("Change"):</td>
    <td>$this->modification</td>
  </tr>
  <tr>
    <td>translate("New"):</td>
    <td>$this->new_text</td>
  </tr>
</table>
<br>
<table>
  <?php for ($this->notes as $offset => $note) {
    <tr>
      <td>
      <?php if ($this->live_notes_editor) {
        <a class="opennote" href="$this->notes[$offset]">$this->summaries[$offset]</a>
      <?php } else {
        <a href="../notes/note?id=$this->notes[$offset]" ##targetblank##>$this->summaries[$offset]</a>
      <?php }
      </td>
      <td><?php if ($this->subscriptions[$offset]) {<a href="unsubscribe" id="unsubscribe$this->notes[$offset]">[translate("unsubscribe")]</a><?php }</td>
      <td><?php if ($this->assignments[$offset]) {<a href="unassign" id="unassign$this->notes[$offset]">[translate("I have done my part on it")]</a><?php }</td>
      <td><?php if ($this->level >= 5) {<a href="delete" id="delete$this->notes[$offset]">[translate("mark for deletion")]</a><?php }</td>
    </tr>
  <?php }
</table>
<p>
  $this->timestamp
  |
  <?php if ($this->live_notes_editor) {
    <a class="newnote" href="$this->id">translate("Create note")</a>
  <?php } else {
    <a href="../notes/create?fromchange=$this->id" ##targetblank##>translate("Create note")</a>
  <?php }
</p>
<br>
