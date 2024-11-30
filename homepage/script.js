function addTask(taskText){
    let li = document.createElement("li");
    li.className = "list-group-item";
    li.appendChild(document.createTextNode(taskText));

    let deleteButton = document.createElement("button");
    deleteButton.className = "btn btn-danger btn-sm float-right delete";
    deleteButton.appendChild(document.createTextNode("Delete!"));

    li.appendChild(deleteButton);
    document.getElementById("task-list").appendChild(li);

}

document.getElementById("task-form").addEventListener("submit", function(e){
    e.preventDefault();

    let taskInput = document.getElementById("task-input");
    let taskText = taskInput.value.trim();

    if (taskText!==""){
        addTask(taskText);
        taskInput.value="";
    } else {
        alert("Please enter a task!!");
    }
});


document.getElementById("task-list").addEventListener("click", function(e){
    if (e.target.classList.contains("delete")){
        e.target.parentElement.remove();
    }
});


function saveSettings(){
    let backgroundColor = document.getElementById("background-color").value;
    document.body.style.backgroundColor = backgroundColor;

}

document.addEventListener("DOMContentLoaded", function() {
    document.getElementById("settings-form").addEventListener("submit", function(e) {
        e.preventDefault();
        saveSettings();
        alert("Settings saved!");
    });
});
