class ColorPicker {
    
    constructor(root) {
        this.root = root;
        this.colorjoe = colorjoe.rgb(this.root.querySelector(".colorjoe"));
        this.suppressUpdate = false; // Flag to prevent updates from triggering WebSocket sends

        // Event listener for color change
        this.colorjoe.on('change', (color) => {
            if (!this.suppressUpdate) {
                this.updateColor(color);
            }
        });

        this.colorjoe.show();
    }

    updateColor(color) {
        // Extract the RGB values from the color object
        const { r, g, b } = hexToRgb(color.hex());

        updateUI(r, g, b);
    }

    refreshColorPicker(r, g, b) {
        this.suppressUpdate = true; // Suppress updates to prevent WebSocket send
        const hex_color = rgbToHex(r, g, b);
        this.colorjoe.set(hex_color);
        this.suppressUpdate = false; // Allow updates again
    }

}

// Initialize Color Picker
cp = new ColorPicker(document.querySelector(".colorPickerContainer"));

// Define WebSocket variable
let socket;

// Function to initialize WebSocket
function initWebSocket() {
    // Create a new WebSocket connection to your ESP8266 server
    socket = new WebSocket(`ws://${window.location.hostname}/ws`);

    // Handle the connection opening
    socket.addEventListener('open', () => {
        console.log('WebSocket connection established');
    });

    // Handle errors
    socket.addEventListener('error', (error) => {
        console.error('WebSocket Error:', error);
    });

    // Handle connection closure
    socket.addEventListener('close', () => {
        console.log('WebSocket connection closed');
    });
}

function hexToRgb(hex) {
    const bigint = parseInt(hex.replace(/^#/, ''), 16);
    return {
        r: (bigint >> 16) & 255,
        g: (bigint >> 8) & 255,
        b: bigint & 255
    };
}

function rgbToHex(r, g, b) {
    return  '#' + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1).toUpperCase();
}

const rSlider = document.getElementById('rSlider');
const gSlider = document.getElementById('gSlider');
const bSlider = document.getElementById('bSlider');
const rValue = document.getElementById('rValue');
const gValue = document.getElementById('gValue');
const bValue = document.getElementById('bValue');
const colorPreview = document.getElementById('colorPreview');

function sendRGBValues(r, g, b) {
    // Send RGB values to the ESP8266 over WebSocket
    if (socket && socket.readyState === WebSocket.OPEN) {
        const rgbData = JSON.stringify({ r: parseInt(r), g: parseInt(g), b: parseInt(b) }); // Convert RGB values to JSON
        socket.send(rgbData); // Send the RGB data
        console.log("RGB data sent:", rgbData);
    } else {
        console.warn('WebSocket connection not open. RGB data not sent.');
    }
}

function debounce(func, wait) {
    let timeout;
    return function(...args) {
        const context = this; // Store the context (this) for use inside the function
        clearTimeout(timeout); // Clear the previous timeout
        timeout = setTimeout(() => {
            func.apply(context, args); // Call the function with the correct context and arguments
        }, wait);
    };
}

const sendRGBValuesDebounced = debounce(sendRGBValues, 20);

function updateUI(r, g, b) {
    requestAnimationFrame(() => {
        // Update sliders
        rSlider.value = r;
        gSlider.value = g;
        bSlider.value = b;

        // Update background color and text
        colorPreview.style.backgroundColor = `rgb(${r}, ${g}, ${b})`;
        colorPreview.textContent = `RGB(${r}, ${g}, ${b})`;

        // Update slider labels
        rValue.textContent = r;
        gValue.textContent = g;
        bValue.textContent = b;

        // Update color picker
        cp.refreshColorPicker(r, g, b);

        // Send RGB values to the ESP8266 over WebSocket using the debounced function
        sendRGBValuesDebounced(r, g, b);
    });
}

function updateColorPreview()
{
    const r = parseInt(rSlider.value, 10);
    const g = parseInt(gSlider.value, 10);
    const b = parseInt(bSlider.value, 10);
    updateUI(r, g, b);
}

rSlider.addEventListener('input', updateColorPreview);
gSlider.addEventListener('input', updateColorPreview);
bSlider.addEventListener('input', updateColorPreview);

document.addEventListener("DOMContentLoaded", function() {
    const colors = [
        '#000000', '#444444', '#5b5b5b', '#999999', '#bcbcbc', '#eeeeee', '#f3f6f4', '#ffffff',
        '#f44336', '#744700', '#ce7e00', '#8fce00', '#2986cc', '#16537e', '#6a329f', '#c90076',
        '#f4cccc', '#fce5cd', '#fff2cc', '#d9ead3', '#d0e0e3', '#cfe2f3', '#d9d2e9', '#ead1dc',
        '#ea9999', '#f9cb9c', '#ffe599', '#b6d7a8', '#a2c4c9', '#9fc5e8', '#b4a7d6', '#d5a6bd',
        '#e06666', '#f6b26b', '#ffd966', '#93c47d', '#76a5af', '#6fa8dc', '#8e7cc3', '#c27ba0',
        '#cc0000', '#e69138', '#f1c232', '#6aa84f', '#45818e', '#3d85c6', '#674ea7', '#a64d79',
        '#990000', '#b45f06', '#bf9000', '#38761d', '#134f5c', '#0b5394', '#351c75', '#741b47',
        '#660000', '#783f04', '#7f6000', '#274e13', '#0c343d', '#073763', '#20124d', '#4c1130'
    ];

    const colorGrid = document.getElementById("colorGrid");

    // Create swatches
    colors.forEach(color => {
        const swatch = document.createElement("div");
        swatch.classList.add("color-swatch");
        swatch.style.backgroundColor = color;
        colorGrid.appendChild(swatch); // Add the swatch to the grid
    });

    // Event delegation: single listener on colorGrid
    colorGrid.addEventListener('click', function(event) {
        // Check if the clicked element is a color swatch
        if (event.target.classList.contains('color-swatch')) {
            const swatchColor = event.target.style.backgroundColor; // Get the swatch's color
            
            // Convert RGB color to r, g, b values
            const rgb = swatchColor.match(/\d+/g).map(Number);
            const [r, g, b] = rgb;

            updateUI(r, g, b);
        }
    });

    updateColorPreview();
    initWebSocket();
});
