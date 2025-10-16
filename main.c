<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Notes / Journal App</title>

  <!-- ===== CSS Styling ===== -->
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
      font-family: "Poppins", sans-serif;
    }

    body {
      background: #f3f6fa;
      color: #333;
      display: flex;
      justify-content: center;
      align-items: flex-start;
      min-height: 100vh;
      padding: 40px 20px;
    }

    .container {
      width: 100%;
      max-width: 700px;
      background: white;
      padding: 25px;
      border-radius: 15px;
      box-shadow: 0 6px 20px rgba(0, 0, 0, 0.1);
    }

    h1 {
      text-align: center;
      margin-bottom: 20px;
      color: #2a5298;
    }

    .note-input {
      width: 100%;
      height: 100px;
      padding: 10px;
      font-size: 16px;
      border: 1px solid #ccc;
      border-radius: 8px;
      outline: none;
      resize: none;
    }

    .buttons {
      display: flex;
      justify-content: flex-end;
      margin-top: 10px;
      gap: 10px;
    }

    button {
      padding: 10px 15px;
      background: #2a5298;
      color: white;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      font-size: 15px;
      transition: 0.3s;
    }

    button:hover {
      background: #1e3c72;
    }

    .notes {
      margin-top: 25px;
    }

    .note {
      background: #eaf1ff;
      padding: 15px;
      border-radius: 8px;
      margin-bottom: 12px;
      position: relative;
      transition: all 0.3s;
    }

    .note:hover {
      background: #dce7ff;
    }

    .note p {
      font-size: 15px;
      white-space: pre-wrap;
      margin-right: 80px;
    }

    .note-buttons {
      position: absolute;
      top: 10px;
      right: 10px;
      display: flex;
      gap: 5px;
    }

    .note-buttons button {
      background: #2a5298;
      color: white;
      border: none;
      padding: 5px 8px;
      border-radius: 6px;
      cursor: pointer;
      font-size: 12px;
      transition: 0.2s;
    }

    .note-buttons button:hover {
      opacity: 0.8;
    }

    .delete-btn {
      background: crimson;
    }

    .edit-btn {
      background: #2a5298;
    }

    @media (max-width: 500px) {
      .container {
        padding: 20px;
      }
      button {
        width: 100%;
      }
      .note p {
        margin-right: 0;
      }
    }
  </style>
</head>
<body>

  <!-- ===== App Container ===== -->
  <div class="container">
    <h1>📝 My Notes / Journal</h1>

    <!-- Note Input -->
    <textarea id="noteInput" class="note-input" placeholder="Write your note here..."></textarea>
    <div class="buttons">
      <button id="addNoteBtn">Add Note</button>
      <button id="updateNoteBtn" style="display: none;">Update Note</button>
    </div>

    <!-- Notes Display -->
    <div class="notes" id="notesContainer"></div>
  </div>

  <!-- ===== JavaScript Logic ===== -->
  <script>
    const addNoteBtn = document.getElementById("addNoteBtn");
    const updateNoteBtn = document.getElementById("updateNoteBtn");
    const noteInput = document.getElementById("noteInput");
    const notesContainer = document.getElementById("notesContainer");

    let editNoteId = null; // track which note is being edited

    // Load saved notes when page loads
    document.addEventListener("DOMContentLoaded", loadNotes);

    // Add new note
    addNoteBtn.addEventListener("click", () => {
      const noteText = noteInput.value.trim();
      if (noteText === "") {
        alert("Please write something before adding a note!");
        return;
      }

      const note = { id: Date.now(), text: noteText };
      saveNoteToLocal(note);
      addNoteToUI(note);
      noteInput.value = "";
    });

    // Save note to localStorage
    function saveNoteToLocal(note) {
      const notes = getNotesFromLocal();
      notes.push(note);
      localStorage.setItem("notes", JSON.stringify(notes));
    }

    // Get notes from localStorage
    function getNotesFromLocal() {
      const notes = localStorage.getItem("notes");
      return notes ? JSON.parse(notes) : [];
    }

    // Display notes in UI
    function loadNotes() {
      const notes = getNotesFromLocal();
      notes.forEach(addNoteToUI);
    }

    // Add note to UI
    function addNoteToUI(note) {
      const noteDiv = document.createElement("div");
      noteDiv.classList.add("note");
      noteDiv.setAttribute("data-id", note.id);

      const noteText = document.createElement("p");
      noteText.textContent = note.text;

      const noteButtons = document.createElement("div");
      noteButtons.classList.add("note-buttons");

      // Edit button
      const editBtn = document.createElement("button");
      editBtn.textContent = "Edit";
      editBtn.classList.add("edit-btn");
      editBtn.addEventListener("click", () => editNote(note.id));

      // Delete button
      const deleteBtn = document.createElement("button");
      deleteBtn.textContent = "Delete";
      deleteBtn.classList.add("delete-btn");
      deleteBtn.addEventListener("click", () => deleteNote(note.id));

      noteButtons.appendChild(editBtn);
      noteButtons.appendChild(deleteBtn);

      noteDiv.appendChild(noteText);
      noteDiv.appendChild(noteButtons);

      notesContainer.prepend(noteDiv); // newest note on top
    }

    // Delete note
    function deleteNote(id) {
      let notes = getNotesFromLocal();
      notes = notes.filter((note) => note.id !== id);
      localStorage.setItem("notes", JSON.stringify(notes));

      // Remove from UI
      const noteElement = document.querySelector(`.note[data-id='${id}']`);
      if (noteElement) noteElement.remove();
    }

    // Edit note
    function editNote(id) {
      const notes = getNotesFromLocal();
      const noteToEdit = notes.find((note) => note.id === id);
      if (!noteToEdit) return;

      noteInput.value = noteToEdit.text;
      editNoteId = id;
      addNoteBtn.style.display = "none";
      updateNoteBtn.style.display = "inline-block";
    }

    // Update edited note
    updateNoteBtn.addEventListener("click", () => {
      const updatedText = noteInput.value.trim();
      if (updatedText === "") {
        alert("Note cannot be empty!");
        return;
      }

      let notes = getNotesFromLocal();
      notes = notes.map((note) =>
        note.id === editNoteId ? { ...note, text: updatedText } : note
      );
      localStorage.setItem("notes", JSON.stringify(notes));

      // Refresh UI
      notesContainer.innerHTML = "";
      loadNotes();

      // Reset input and buttons
      noteInput.value = "";
      editNoteId = null;
      addNoteBtn.style.display = "inline-block";
      updateNoteBtn.style.display = "none";
    });
  </script>
</body>
</html>
