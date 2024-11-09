export class Hello extends Element 
{
  componentDidMount() {
    var message = this.attributes["message"] || "?";
    this.content([<h1>{message}</h1>,
                 <button.expand>+</button>,
                 <button.collapse>-</button>]);
  }
  ["on click at button.expand"]() {   // note: ES2020 syntax
    this.state.expanded = true;       // 'this' is the element of class Hello
  }
  ["on click at button.collapse"]() { // handles click events
    this.state.collapsed = true;      // at button.collapse child
  }
}